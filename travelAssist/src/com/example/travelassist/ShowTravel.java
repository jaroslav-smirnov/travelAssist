package com.example.travelassist;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.example.travelassist.DBHelper.Tables;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ExpandableListView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;
import android.widget.Toast;

public class ShowTravel extends FragmentActivity implements ItemRemovable {

	int idInList;
	int Type;
	DBHelper dbHelper;
	float totalAmount;
	// EditText etOut;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		setContentView(R.layout.result_view);
		// etOut = (EditText) findViewById(R.id.editText1);
		
		ActionBar actionBar;
		actionBar = getActionBar();
		Log.i("tab7", "set new title");
		actionBar.setTitle(R.string.labTrips);
		actionBar.setDisplayHomeAsUpEnabled(true);
		
		dbHelper = new DBHelper(this);
		idInList = intent.getIntExtra("travelId", 0);
		Type = getIdByPos(idInList);
		/*Toast.makeText(getApplicationContext(), String.valueOf(Type),
				Toast.LENGTH_LONG).show();*/

		Receipt staRec = new Receipt();

		Receipt[] receipts = staRec.getListByTravelId(Type);

		for (int i = 0; i < receipts.length; i++) {
			Log.d("receipt", receipts[i].toString());
		}
		
		travelRecord tr1 = new travelRecord(Type);
		CurrencyConverter cconv = new CurrencyConverter();
		// Log.d("currencies are", cconv.toString());

		// Log.d("Travel name", tr1.getName());
		// Log.d("Travellers", tr1.getNamesStr());
		
		TextView tv = (TextView)findViewById(R.id.textView1);

		SQLiteDatabase db = dbHelper.getReadableDatabase();
		AllCurrencies cur = AllCurrencies.getInstance(db);
		db.close();
		String basicCurName = cur.getNameById(1);
		
		tv.setText(tr1.getName());
		int totalUsers;
		int[] users;
		totalUsers = tr1.getTrCount();
		float[][] howManyMoney = new float[totalUsers][totalUsers];

		// Log.d("Travellers numbers", tr1.getNamesStr());
		int ii;
		int jj;

		/*
		 * Log.d("test 1", String.valueOf(tr1.getPersPositionById(1)));
		 * Log.d("test 4", String.valueOf(tr1.getPersPositionById(4)));
		 * Log.d("test 2", String.valueOf(tr1.getPersPositionById(2)));
		 * Log.d("test 3", String.valueOf(tr1.getPersPositionById(3)));
		 */
		for (int i = 0; i < receipts.length; i++) {
			users = receipts[i].getWhoUse();
			
			for (int j = 0; j < users.length; j++) {
				
				ii = tr1.getPersPositionById(receipts[i].getBuyer()); 
				jj = tr1.getPersPositionById(users[j]);
			
				totalAmount += receipts[i].getPart() * cconv.getScale(receipts[i].getCurrency());
				
				if (ii != jj)
					howManyMoney[ii][jj] += receipts[i].getPart()
							* cconv.getScale(receipts[i].getCurrency());
				else
					howManyMoney[ii][jj] = 0;
			}
		}

		TextView tv2 = (TextView)findViewById(R.id.textView2);
		tv2.setText(getString(R.string.total_str)+totalAmount + " " + basicCurName);
		for (int i = 0; i < totalUsers; i++) {
			for (int j = 0; j < totalUsers; j++) {
				if ((howManyMoney[i][j] >= howManyMoney[j][i])) {
					howManyMoney[i][j] = howManyMoney[i][j]
							- howManyMoney[j][i];
					howManyMoney[j][i] = 0;
				}
			}
		}
		ArrayList<Map<String, String>> groupData = new ArrayList<Map<String, String>>();
		Map<String, String> m;
		for (int i = 0; i < totalUsers; i++) {
			m = new HashMap<String, String>();
			m.put("groupName", tr1.getTraveller(i));
			groupData.add(m);
		}

		String groupFrom[] = new String[] { "groupName" };
		int groupTo[] = new int[] { android.R.id.text1 };

		ArrayList<ArrayList<Map<String, String>>> childData = new ArrayList<ArrayList<Map<String, String>>>();

		ArrayList<Map<String, String>> childDataItem = new ArrayList<Map<String, String>>();

		
		for (int i = 0; i < totalUsers; i++) {
			childDataItem = new ArrayList<Map<String, String>>();
			for (int j = 0; j < totalUsers; j++) {
				if (i != j) {
					if (howManyMoney[i][j] != 0) {
						m = new HashMap<String, String>();
						m.put("itemName",getString(R.string.from_string)+
								tr1.getTraveller(j)
										+ " +"
										+ String.format("%.2f",
												howManyMoney[i][j]) + " " + basicCurName);
						childDataItem.add(m);
					}
					if (howManyMoney[j][i] != 0) {
						m = new HashMap<String, String>();
						m.put("itemName",getString(R.string.to_string)+
								tr1.getTraveller(j)
										+ " -"
										+ String.format("%.2f",
												howManyMoney[j][i]) + " " + basicCurName);
						childDataItem.add(m);
						
					}
				}
			}
			childData.add(childDataItem);
		}
		String childFrom[] = new String[] { "itemName" };
		int childTo[] = new int[] { android.R.id.text1 };

		SimpleExpandableListAdapter adapter = new SimpleExpandableListAdapter(
				this, groupData,
				android.R.layout.simple_expandable_list_item_1, groupFrom,
				groupTo, childData, android.R.layout.simple_list_item_1,
				childFrom, childTo);
		ExpandableListView elvMain = (ExpandableListView) findViewById(R.id.expandableListView1);
		elvMain.setAdapter(adapter);

	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.menu_for_view, menu);
		return true;
	}
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {

		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			break;
		case R.id.item_add:
			Intent intent = new Intent(this, addBuy.class);
			intent.putExtra("typeOfAdd", 1);
			startActivityForResult(intent, 0);
			break;
		case R.id.item_edit:
			intent = new Intent(this, changeTravel.class);
			intent.putExtra("id",idInList );
			startActivityForResult(intent, 0);
			break;
		case R.id.item_remove:
			RemoveConfirmationDialogFragment newFragment = new RemoveConfirmationDialogFragment();
			newFragment.id = Type;
			newFragment.setElement(this);
			newFragment.show(getSupportFragmentManager(), "dsfs");

			break;
		}
		return super.onOptionsItemSelected(item);
	}
	
	@Override
	protected void onActivityResult(int a, int b, Intent c)
	{
		TextView tv = (TextView)findViewById(R.id.textView1);


		travelRecord tr1 = new travelRecord(Type);
		
		SQLiteDatabase db = dbHelper.getReadableDatabase();
		db.close();
		tv.setText(tr1.getName());
	}


	public int getIdByPos(int pos) {

		SQLiteDatabase db = dbHelper.getReadableDatabase();
		int idr;
		Cursor cur = db.query("travels", null, null, null, null, null, null);
		int position = cur.getCount() - pos;
		// Log.d("positiion", String.valueOf(position));
		if (cur.move(position)) {
			int idColIndex = cur.getColumnIndex("id");
			// int idNameIndex = cur.getColumnIndex("name");
			idr = cur.getInt(idColIndex);
		} else {
			Log.d("db", "No records");
			idr = 0;
		}
		cur.close();
		db.close();
		return idr;

	}

	class travelRecord {
		private int id;
		private String name;

		private String[] names;
		Person stPerson;

		private Person[] persons;

		public travelRecord(int ide) {
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			stPerson = new Person(0, "no person");

			Cursor cur = db.query("travels", null,
					"ID = " + String.valueOf(ide), null, null, null, null);
			// Log.d("We have got " + "id = " + String.valueOf(ide),
			// String.valueOf(cur.getCount()));
			this.name = "";
			if (cur.moveToFirst()) {
				// Log.d("We have record", "record");
				int idColIndex = cur.getColumnIndex("id");
				int idNameIndex = cur.getColumnIndex("name");
				id = cur.getInt(idColIndex);
				name = cur.getString(idNameIndex);

				// Log.d("our id is and name", String.valueOf(id) + " " + name);
				String travlrs = cur.getString(cur.getColumnIndex("travelers"));
				// Log.d("travlrs are", travlrs);
				persons = stPerson.personsFromString(travlrs);
				// Log.d("Was returned", String.valueOf(persons.length));
			} else
				Log.d("db", "No records");
			cur.close();
			db.close();

		}

		public String[] getNames() {
			return names;
		}

		public Person[] getPersons() {
			return persons;
		}

		public int getTrCount() {
			return persons.length;
		}

		public String getNamesStr() {
			String buf = new String();
			for (int i = 0; i < persons.length; i++)
				buf += String.valueOf(persons[i].getId()) + ":"
						+ persons[i].getName() + ";";
			return buf;
		}

		public String getTraveller(int id) {
			return persons[id].getName();
		}

		public String getName() {
			return name;
		}

		public int getId() {
			return id;
		}

		public int getPersPositionById(int ido) {
			for (int i = 0; i < persons.length; i++) {
				// Log.d(String.valueOf(persons[i].getId()),
				// String.valueOf(persons[i].getName()));
				if (persons[i].getId() == ido)
					return i;
			}
			return -1;
		}

	}

	class Person {
		private int _id;
		private String _personName;

		Person(int id, String name) {
			_id = id;
			_personName = name;
		}

		Person(int id) {
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			Cursor cur = db.query("person", null, "id = " + String.valueOf(id),
					null, null, null, null);
			if (cur.moveToFirst()) {
				int idColIndex = cur.getColumnIndex("id");
				int idNameIndex = cur.getColumnIndex("name");
				_id = cur.getInt(idColIndex);
				_personName = cur.getString(idNameIndex);
			} else {
				Log.d("db", "No records");
				_id = 0;
				_personName = "";
			}
			Log.d("New person", this.getName());
			Log.d("With id", String.valueOf(this.getId()));
			cur.close();
			db.close();
		}

		int getId() {
			return _id;
		}

		String getName() {
			return _personName;
		}

		public Person[] personsFromString(String str) {
			String pers[] = str.split(",");
			Person persons[] = new Person[pers.length];
			Log.d("We should create persons", String.valueOf(pers.length));
			for (int i = 0; i < pers.length; i++) {
				persons[i] = new Person(Integer.parseInt(pers[i].trim()));
			}

			return persons;
		}

	}

	class Receipt {
		private float _sum;
		private int _whoPay;
		private int[] _whoUse;
		private int _currency;

		Receipt() {
			_sum = 0;
			_whoPay = 0;
			_whoUse = new int[1];
			_whoUse[0] = 0;
			_currency = 0;

		}

		Receipt(float sum, int whoPay, String whoUse, int currency) {
			_sum = sum;
			_whoPay = whoPay;
			_currency = currency;
			Log.d("String for parsing", whoUse);
			String[] whouses = whoUse.split(",");
			_whoUse = new int[whouses.length];

			for (int i = 0; i < whouses.length; i++)
				try {
					_whoUse[i] = Integer.parseInt(whouses[i].trim());
					// Log.d(tag, msg)
				} catch (NumberFormatException e) {
					_whoUse[i] = 0;
				}

		}

		@Override
		public String toString() {
			String buffer = new String();
			Log.d("Create new String", "Create");
			Log.d("There are positions", String.valueOf(_whoUse.length));
			for (int i = 0; i < _whoUse.length; i++)
				buffer += String.valueOf(_whoUse[i]) + "|";

			return String.valueOf(_sum) + ":" + String.valueOf(_whoPay) + ":"
					+ buffer + ":" + String.valueOf(_currency) + ";";

		}

		public float getPart() {
			return _sum / _whoUse.length;
		}

		public int getBuyer() {
			return _whoPay;
		}

		public int getCurrency() {
			return _currency;
		}

		public float getSum() {
			return _sum;
		}

		Receipt[] getListByTravelId(int id) {

			SQLiteDatabase db = dbHelper.getWritableDatabase();

			Cursor cur = db.query("buys", null,
					"travel = " + String.valueOf(id), null, null, null, null);
			Receipt recArray[];

			if (cur.moveToLast()) {
				int idNameIndex = cur.getColumnIndex("value");
				// int idId = cur.getColumnIndex("id");
				int idWhoPay = cur.getColumnIndex("whobuy");
				int idWhoUse = cur.getColumnIndex("whouse");
				int idCurr = cur.getColumnIndex("currency");
				int a = 0;
				recArray = new Receipt[cur.getCount()];
				do {
					recArray[a] = new Receipt(cur.getFloat(idNameIndex),
							cur.getInt(idWhoPay), cur.getString(idWhoUse),
							cur.getInt(idCurr));
					a++;
					Log.d("buyer of receipt" + String.valueOf(a),
							String.valueOf(cur.getInt(idWhoPay)));
				} while (cur.moveToPrevious());
			} else {
				Log.d("db", "No records in curr");
				recArray = new Receipt[0];
			}

			cur.close();
			db.close();
			return recArray;
			// return new Receipt[10];
		}

		public int[] getWhoUse() {
			return _whoUse;
		}
	}

	class CurrencyConverter {
		int[] _id;
		float[] _scale;

		CurrencyConverter() {
			Log.d("create converter", "before database");
			SQLiteDatabase db = dbHelper.getWritableDatabase();

			Cursor cur = db.query("currency", null, null, null, null, null,
					null);

			if (cur.moveToLast()) {
				int idScaleIndex = cur.getColumnIndex("usdratio");
				int idId = cur.getColumnIndex("id");
				Log.d("id field", String.valueOf(idId));
				Log.d("Scale field", String.valueOf(idScaleIndex));
				_id = new int[cur.getCount()];
				_scale = new float[cur.getCount()];
				int a = 0;
				// recArray = new Receipt[cur.getCount()];
				do {
					_id[a] = cur.getInt(idId);
					_scale[a] = cur.getFloat(idScaleIndex);
					a++;
				} while (cur.moveToPrevious());
			} else {
				Log.d("db", "No records in curr");
			}

			cur.close();
			db.close();
		}

		public float getScale(int id) {
			for (int i = 0; i < _scale.length; i++)
				if (_id[i] == id)
					return _scale[i];
			return 1;
		}

		public float getScaled(int id, float value) {
			return value * getScale(id);
		}

		@Override
		public String toString() {
			// Log.d("Create new String", "Create");
			String buffer = new String();
			for (int i = 0; i < _id.length; i++)
				buffer += String.valueOf(_id[i]) + ":"
						+ String.valueOf(_scale[i]) + ";";
			return buffer;
		}

	}
	
	

	public static class RemoveConfirmationDialogFragment extends DialogFragment
			implements OnClickListener {
		private ItemRemovable element;
		int id;
		int position;

		public void setElement(ItemRemovable element) {
			this.element = element;
		}

		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			// LayoutInflater inflater = getActivity().getLayoutInflater();
			builder.setMessage(R.string.remove_for_notify_text)
					.setTitle(R.string.remove_for_notify)
					.setPositiveButton(R.string.strOk, this)
					.setNegativeButton("Cancel", null);
			return builder.create();
		}

		@Override
		public void onClick(DialogInterface arg0, int arg1) {
			// 
			element.remove(id, position);
		}

	}



	@Override
	public void remove(int id, int position) {
		SQLiteDatabase db = dbHelper.getWritableDatabase();
		if (DBHelper.removeItem(db, Tables.BUYS, "travel", id,
				Tables.TRIPS, id)) {
		finish();	
		} else
			Toast.makeText(
					this,
					this.getResources().getStringArray(R.array.delete_problems)[position],
					Toast.LENGTH_SHORT).show();
		db.close();
	}


}
