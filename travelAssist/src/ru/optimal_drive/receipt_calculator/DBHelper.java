package ru.optimal_drive.receipt_calculator;

import java.sql.Types;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import ru.optimal_drive.receipt_calculator.R;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.widget.SimpleAdapter;

public class DBHelper extends SQLiteOpenHelper {
	public DBHelper(Context context) {
		super(context, "trAssistDB", null, 1);
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		db.execSQL("create table buys ("
				+ "id integer primary key autoincrement," + "value real, "
				+ "currency integer," + "travel integer," + "desc text,"
				+ "whobuy integer," + "whouse text)");

		db.execSQL("create table travels ("
				+ "id integer primary key autoincrement," + "name text, "
				+ "travelers text)");

		db.execSQL("create table person ("
				+ "id integer primary key autoincrement," + "name text)");

		db.execSQL("create table currency ("
				+ "id integer primary key autoincrement," + "name text, "
				+ "shortname text," + "usdratio real)");
	
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

	}

	public static enum Tables {
		BUYS, TRIPS, PERSONS, CURRENCIES
	}

	/*
	 * public static enum Types { INTT, DOUBLET, TEXTT }
	 */

	public static String getTableName(Tables table) {
		switch (table) {
		case BUYS:
			return "buys";
		case TRIPS:
			return "travels";
		case PERSONS:
			return "person";
		case CURRENCIES:
			return "currency";
		default:
			return null;
		}
	}

	public static String[] getFieldNames(Tables table) {
		switch (table) {
		case BUYS:
			return new String[] { "id", "value", "currency", "travel", "desc",
					"whobuy", "whouse" };
		case TRIPS:
			return new String[] { "id", "name", "travelers" };
		case PERSONS:
			return new String[] { "id", "name" };
		case CURRENCIES:
			return new String[] { "id", "name", "shortname", "usdratio" };
		default:
			return null;
		}
	}

	public static int[] getFieldTypes(Tables table) {
		switch (table) {
		case BUYS:
			return new int[] { Types.INTEGER, Types.DOUBLE, Types.CLOB,
					Types.VARCHAR, Types.CHAR, Types.INTEGER, Types.CHAR };
		case TRIPS:
			return new int[] { Types.INTEGER, Types.CHAR, Types.CHAR };
		case PERSONS:
			return new int[] { Types.INTEGER, Types.CHAR };
		case CURRENCIES:
			return new int[] { Types.INTEGER, Types.CHAR, Types.CHAR,
					Types.DOUBLE };
		default:
			return null;
		}
	}

	public static Cursor getCursor(Tables table, int idT, SQLiteDatabase db) {
		return getCursor(table, "id = " + idT, db);
	}

	public static Cursor getCursor(Tables table, String whereString,
			SQLiteDatabase db) {
		return db.query(DBHelper.getTableName(table), null, whereString, null,
				null, null, null);
	}

	public static Cursor getCursor(Tables table, SQLiteDatabase db) {
		return getCursor(table, null, db);
	}

	public static SimpleAdapter getAdapter(Context ctx, SQLiteDatabase db,
			Tables table) {
		switch (table) {
		case BUYS:
			return getAdapter(ctx, db, Tables.BUYS, "1,2", "3,4");
		case TRIPS:
			return getAdapter(ctx, db, Tables.TRIPS, "1", "");
		case PERSONS:
			return getAdapter(ctx, db, Tables.PERSONS, "1", "");
		case CURRENCIES:
			return getAdapter(ctx, db, Tables.CURRENCIES, "2,3", "1");
		default:
			throw new RuntimeException("Not valid tablename");

		}
	}

	public static SimpleAdapter getAdapter(Context ctx, SQLiteDatabase db,
			Tables table, String mainField, String addFields) {
		Cursor cur = DBHelper.getCursor(table, db);
		int size = cur.getCount();

		final String mainFieldName = "MAINFIELD";
		final String addFieldName = "ADDFIELD";
		final String[] FROM = { mainFieldName, addFieldName };
		final int[] TO = { R.id.textView1, R.id.textView2 };

		int[] mainItems = stringToArray(mainField);
		// if (addFields != null)
		int[] addItems = stringToArray(addFields);

		int[] fieldTypes = getFieldTypes(table);
		int ids = 0;

		ArrayList<Map<String, Object>> data = new ArrayList<Map<String, Object>>(
				size);
		Map<String, Object> m;

		int idItems[] = new int[size];

		if (cur.moveToLast()) {
			int[] mainIndexes = getIndexById(mainItems, cur, table);
			int[] addIndexes = getIndexById(addItems, cur, table);
			int idIndex = cur.getColumnIndex("id");
			do {
				idItems[ids++] = cur.getInt(idIndex);
				m = new HashMap<String, Object>();
				String mainString = "", addString = "";
				for (int i = 0; i < mainIndexes.length; i++)
					mainString += getStringByIndex(db, cur,
							fieldTypes[mainItems[i]], mainIndexes[i]) + " ";
				m.put(mainFieldName, mainString);

				for (int i = 0; i < addIndexes.length; i++)
					addString += getStringByIndex(db, cur,
							fieldTypes[addItems[i]], addIndexes[i]) + " ";

				m.put(addFieldName, addString);

				data.add(m);
			} while (cur.moveToPrevious());
		} else
			{
			//throw new RuntimeException("No records in table currencies");
			}
		cur.close();

		if (addItems.length == 0)
			return new ExSimpleAdapter(ctx, data, R.layout.item_1, FROM, TO,
					idItems);
		return new ExSimpleAdapter(ctx, data, R.layout.item, FROM, TO, idItems);
	}

	public static int[] stringToArray(String str) {

		String[] stringAr = str.split(",");
		if (str.length() == 0)
			return new int[0];
		int[] outAr = new int[stringAr.length];
		for (int i = 0; i < stringAr.length; i++) {
			outAr[i] = Integer.parseInt(stringAr[i].trim());
		}
		return outAr;
	}

	private static int[] getIndexById(int[] id, Cursor cur, Tables table) {
		String[] fieldNames = getFieldNames(table);
		int[] retBuf = new int[id.length];
		for (int i = 0; i < id.length; i++)
			retBuf[i] = cur.getColumnIndex(fieldNames[id[i]]);
		return retBuf;
	}

	public static String getStringByIndex(SQLiteDatabase db, Cursor cur,
			int type, int index) {

		switch (type) {
		case Types.INTEGER:
			return String.valueOf(cur.getInt(index));
		case Types.DOUBLE:
			return String.format("%.1f",Double.valueOf(cur.getDouble(index)));
		case Types.CHAR:
			return cur.getString(index);
		case Types.CLOB: {

			AllCurrencies aCurrency = AllCurrencies.getInstance(db);
			return aCurrency.getNameById(cur.getInt(index));
		}
		case Types.VARCHAR: {
			AllTripNames aTravel = AllTripNames.getInstance(db);
			return aTravel.getNameById(cur.getInt(index)) + ". ";
		}
		case Types.BIT:
		{
			AllTravellers aTravel = AllTravellers.getInstance(db);
			return aTravel.getNameById(cur.getInt(index));
		}
		default:
			throw new RuntimeException("Unsupported type");
		}
	}

	public static boolean removeItem(SQLiteDatabase db, Tables searchTable,
			String likeString, Tables removeTable, int id) {
		Cursor cur = getCursor(searchTable, likeString, db);
		if (cur.getCount() == 0) {
			db.delete(getTableName(removeTable), "id = "+id, null);
			//Log.d("tag2", "Item deleted");
			cur.close();
			return true;
		}
		cur.close();
		return false;
	}
	public static boolean removeItem(SQLiteDatabase db, Tables table, int id)
	{
		return removeItem(db, table,
				"id = -1", table, id);
	}
	
	public static boolean removeItem(SQLiteDatabase db, Tables searchTable, String field, int fId,
			 Tables removeTable, int id) {
		return removeItem(db, searchTable,
				field +" = "+fId, removeTable, id);
	}
	public static boolean removeItem(SQLiteDatabase db, Tables searchTable, String field, String numb,
			 Tables removeTable, int id) {
		return removeItem(db, searchTable,
				field +" LIKE \"% "+numb.trim()+" %\"", removeTable, id);
	}
	
	public static int [] getNeighborId(int id, Tables table, SQLiteDatabase db)
	{
		return getNeighborId(id, table,db,"");
	}
	public static int [] getNeighborId(int id, Tables table, SQLiteDatabase db, String field, int param)
	{
		return getNeighborId(id, table,db,field + "=" + param);
	}
	
	private static int[] getNeighborId(int id, Tables table, SQLiteDatabase db, String condition)
	{
		Cursor cur = getCursor(table, "id = " + id +" AND " + condition, db);
		int nextId = id;
		int prevId = id;
		if (cur.getCount() > 1) throw new RuntimeException("Invalid id count exception");
		if (cur.getCount() == 0) throw new RuntimeException("No item with this id");
		/*
		 * Trying to get next id
		 */
		cur = getCursor(table, "id > " + id +" AND " + condition, db);
		if (cur.getCount() > 0)
		{
			cur.moveToFirst();
			nextId = cur.getInt(cur.getColumnIndex("id"));
		}
		/*
		 * Trying to get previous id
		 */
		cur = getCursor(table, "id > " + id +" AND " + condition, db);
		if (cur.getCount() > 0)
		{
			cur.moveToLast();
			nextId = cur.getInt(cur.getColumnIndex("id"));
		}
		return new int[] {prevId, nextId};
	}
	

}
