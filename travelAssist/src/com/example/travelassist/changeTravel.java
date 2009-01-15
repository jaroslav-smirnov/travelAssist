package com.example.travelassist;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.util.SparseBooleanArray;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

public class changeTravel extends Activity {

	DBHelper dbHelper;
	int trIndexStorage[];
	editableTravel eTr;
	int id;
	@Override
	public void onBackPressed() {
		new AlertDialog.Builder(this)
		.setIcon(android.R.drawable.ic_dialog_alert)
		.setTitle(R.string.str_changes_save)
		//.setMessage(R.string.str_are_you_sure)
		.setPositiveButton(R.string.yes, new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				finish();
			}

		}).setNegativeButton(R.string.no, null).show();

	}

	// editableTravel eTr;
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.add_trip);
		dbHelper = new DBHelper(this);
		getTravellers();
		ListView elvTravellers = (ListView) findViewById(R.id.listView1);
		Intent intent = getIntent();
		id = intent.getIntExtra("id", 0);
		id = getIdByPos(id, "travels");

		ActionBar actionBar;
		actionBar = getActionBar();
		actionBar.setDisplayHomeAsUpEnabled(true);
		Log.i("tab7", "set new title");
		actionBar.setTitle(R.string.labTrips);
		eTr = new editableTravel(id);

		EditText etxtName = (EditText) findViewById(R.id.editText1);
		etxtName.setText(eTr.getName());

		for (int i = 0; i < trIndexStorage.length; i++) {
			// Log.d("Item "+ String.valueOf(i), String.valueOf(eTr.hasPerson(
			// trIndexStorage[i])));
			if (eTr.hasPerson(trIndexStorage[i]))
				elvTravellers.setItemChecked(i, true);
		}

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.getdata, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			break;
		case R.id.item_accept:
			ListView elvTravellers = (ListView) findViewById(R.id.listView1);
			if (eTr.isAffectedRecords(eTr
					.getPersonsStr(), eTr.createStringFromLst(elvTravellers
					.getCheckedItemPositions())))
			{
				Toast.makeText(getApplicationContext(),
						getString(R.string.toast_person_is_used),Toast.LENGTH_SHORT).show();
				return true;
			}
			EditText nameStr =(EditText)findViewById(R.id.editText1);
			if (nameStr.getText().toString().isEmpty()) 
			{
				Toast.makeText(getApplicationContext(),
						getString(R.string.name_is_empty),Toast.LENGTH_SHORT).show();
				return true;
			}
				
				
			eTr.setName(nameStr.getText().toString());
			eTr.setPersonsStr(eTr.createStringFromLst(elvTravellers.getCheckedItemPositions()));
			
			eTr.writeChanges();
			finish();

			break;
		case R.id.item_reset:
			finish();
			break;
		}
		return super.onOptionsItemSelected(item);

	}

	class editableTravel {
		private String _travelName;
		private int _id;
		private Person[] _persons;
		private String _personsStr;

		int getPersNumber() {
			return _persons.length;
		}

		String getName() {
			return _travelName;
		}

		String getPersonsStr() {
			return _personsStr;
		}
		
		void setPersonsStr(String persons)
		{
			_personsStr = persons;
		}
		
		void setName(String travelName)
		{
			_travelName = travelName;
		}

		boolean hasPerson(int id) {
			Log.d("We have several persons", "");
			for (int i = 0; i < _persons.length; i++) {
				// Log.d("check person number", String.valueOf(i));
				if (_persons[i].getId() == id)
					return true;
			}
			return false;
		}
		
		void writeChanges()
		{
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			ContentValues cv = new ContentValues();
			cv.put("name", _travelName);
			cv.put("travelers", this.getPersonsStr());
			db.update("travels", cv, "id = " + String.valueOf(_id), null);
			db.close();
		}

		editableTravel(int id) {
			_id = id;
			String personsStr;
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			Cursor cur = db.query("travels", null,
					"id = " + String.valueOf(id), null, null, null, null);
			if (cur.moveToFirst()) {
				int idColIndex = cur.getColumnIndex("id");
				int idNameIndex = cur.getColumnIndex("name");
				int idPersonsIndex = cur.getColumnIndex("travelers");
				_id = cur.getInt(idColIndex);
				_travelName = cur.getString(idNameIndex);
				personsStr = cur.getString(idPersonsIndex);
				_personsStr = personsStr;
				Log.d("persons are", personsStr);
			} else {
				Log.d("db", "No records");
				_id = 0;
				_travelName = "";
				personsStr = "";
			}
			cur.close();
			db.close();
			int indexes[];
			// try
			// {
			String[] buferIndxStr = personsStr.split(",");
			indexes = new int[buferIndxStr.length];
			_persons = new Person[indexes.length];
			for (int i = 0; i < indexes.length; i++) {
				indexes[i] = Integer.parseInt(buferIndxStr[i].trim());
				_persons[i] = new Person(indexes[i]);
				Log.d(String.valueOf(i), _persons[i].getName());
			}
			/*
			 * } catch (NumberFormatException e) {
			 * 
			 * }
			 */

		}

		private boolean isAffectedRecords(String oldPersons, String newPersons) {
			// int newCount = oldPersons.split(",").length;
			// int oldCount = newPersons.split(",").length;

			Log.d(oldPersons, newPersons);
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			String[] oldItems = oldPersons.split(",");
			// boolean willHave [] = new boolean [oldItems.length];

			Cursor cur;
			
			cur = db.query("buys", null, null, null,null, null, null);
			
			
			if (cur.moveToLast()) {
				int idColIndex = cur.getColumnIndex("whouse");
				do {
					Log.d("whouse","|"+ cur.getString(idColIndex)+"|");
				} while (cur.moveToPrevious());
			}
			
			
			for (int i = 0; i < oldItems.length; i++) {
				if (!newPersons.contains(" " + oldItems[i].trim() + " ")) {
					cur = db.query(
							"buys",
							null,
							"(travel = "
									+ String.valueOf(_id)
									+ ") AND "
									+ " (whobuy = "
									+ oldItems[i].trim() + ")"
									, null, null,
							null, null);
					Log.d("result whobuy", String.valueOf(cur.getCount()));
					if (cur.getCount() > 0)
					{   cur.close();
					    db.close();
						return true;
					}
					else cur.close();
					Log.d("who use", "|"+oldItems[i].trim()+"|");
					cur = db.query(
							"buys",
							null,
							"(travel = "
									+ String.valueOf(_id)
									+ ") AND (" +
									 " whouse LIKE \"% " 
									+ oldItems[i].trim() + " %\""
									+")"
									, null, null,
							null, null);
					Log.d("result use", String.valueOf(cur.getCount()));
					
					if (cur.getCount() > 0)
					{   
						cur.close();
					    db.close();
						return true;
					}
					else cur.close();
				}
			}
			
			db.close();
			return false;
		}

		String createStringFromLst(SparseBooleanArray sbArray) {
			String buffer = new String();
			boolean isFirst = true;
			Log.d("elements in array", String.valueOf(sbArray.size()));
			for (int i = 0; i < sbArray.size(); i++) {
				// // if (sbArray.get(i))
				if (sbArray.valueAt(i)) {

					if (isFirst) {
						buffer = " "
								+ String.valueOf(trIndexStorage[sbArray
										.keyAt(i)]) + " ";
						isFirst = false;
					} else
						buffer += ", "
								+ String.valueOf(trIndexStorage[sbArray
										.keyAt(i)]) + " ";
				}
			}
			return buffer;
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

	}

	public void getTravellers() {
		SQLiteDatabase db = dbHelper.getWritableDatabase();

		Cursor cur = db.query("person", null, null, null, null, null, null);

		// Spinner spnr = (Spinner)findViewById(R.id.spinner1);
		ListView elvTravellers = (ListView) findViewById(R.id.listView1);
		elvTravellers.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);

		// Log.d("jhgjg", "start to work with db");

		if (cur.moveToLast()) {
			int idColIndex = cur.getColumnIndex("id");
			int idNameIndex = cur.getColumnIndex("name");
			trIndexStorage = new int[cur.getCount()];
			String array_spinner1[];
			int i = 0;
			array_spinner1 = new String[cur.getCount()];
			do {
				array_spinner1[i] = cur.getString(idNameIndex);
				trIndexStorage[i] = cur.getInt(idColIndex);

				// array_spinner1[i]+=":"+String.valueOf(trIndexStorage[i]);
				// Log.d("dsfds", array_spinner1[i]);
				i++;
			} while (cur.moveToPrevious());
			ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
					android.R.layout.simple_list_item_multiple_choice,
					array_spinner1);
			// adapter.setDropDownViewResource(android.R.layout.);
			elvTravellers.setAdapter(adapter);
		} else
			Log.d("db", "No records in curr");

		cur.close();
		db.close();

	}

	public int getIdByPos(int pos, String table) {

		SQLiteDatabase db = dbHelper.getReadableDatabase();
		int idr;
		Cursor cur = db.query(table, null, null, null, null, null, null);
		int position = cur.getCount() - pos;
		if (cur.move(position)) {
			int idColIndex = cur.getColumnIndex("id");
			idr = cur.getInt(idColIndex);
		} else {
			Log.d("db", "No records");
			idr = 0;
		}
		cur.close();
		db.close();
		return idr;
	}

}
