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
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.Toast;

public class changePersonAndCurrency extends Activity {
	private boolean isPerson;
	private int id;
	DBHelper dbHelper;
	Person per;
	Currency cur;
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

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    Intent intent = getIntent();
		isPerson = intent.getBooleanExtra("isPerson", false);
		id = intent.getIntExtra("id", 0);
		ActionBar actionBar;
		actionBar = getActionBar();
		actionBar.setDisplayHomeAsUpEnabled(true);

		
		Log.d("Person", String.valueOf(isPerson));
		Log.d("id", String.valueOf(id));
		dbHelper = new DBHelper(this);
		if (isPerson)
		{
			id = getIdByPos(id, "person");
			setContentView(R.layout.new_traveller);
			per = new Person(id);
			EditText name = (EditText)findViewById(R.id.editText1);
			name.setText(per.getName());
		}
		else 
			{
			id = getIdByPos(id, "currency");
			setContentView(R.layout.new_currency);
			cur = new Currency(id);
			EditText name = (EditText)findViewById(R.id.editText1);
			name.setText(cur.getName());
			EditText shortName = (EditText)findViewById(R.id.editText2);
			shortName.setText(cur.getShortName());
			EditText ratio = (EditText)findViewById(R.id.editText3);
			ratio.setText(String.valueOf(cur.getRatio()));
			}
	    // TODO Auto-generated method stub
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.getdata, menu);
		return true;
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
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			break;
		case R.id.item_accept:
			
			//return super.onOptionsItemSelected(item);
			if (isPerson)
			{
				EditText name = (EditText)findViewById(R.id.editText1);
				if (name.getText().toString().isEmpty())
				{
					Toast.makeText(getApplicationContext(), 
							getString(R.string.toast_fill_all_of_the_strings), Toast.LENGTH_SHORT).show();
					return super.onOptionsItemSelected(item);
				}
				per.setName(name.getText().toString());
				per.writeChanges();
			}
			else 
				{
				
				EditText name = (EditText)findViewById(R.id.editText1);
				//name.setText(cur.getName());
				cur.setName(name.getText().toString());
				EditText shortName = (EditText)findViewById(R.id.editText2);
				cur.setShortName(shortName.getText().toString());
				EditText ratio = (EditText)findViewById(R.id.editText3);
				//Float fRatio;
				if (name.getText().toString().isEmpty()||
						shortName.toString().isEmpty()||
						ratio.getText().toString().isEmpty())
				{
					Toast.makeText(getApplicationContext(), getString(R.string.toast_fill_all_of_the_strings), 
							Toast.LENGTH_SHORT).show();
					return super.onOptionsItemSelected(item);
				}
				cur.setRatio(Float.parseFloat(ratio.getText().toString()));
				cur.writeChanges();
				}
			
			finish();

			break;
		case R.id.item_reset:
			finish();
			break;
		}

		return super.onOptionsItemSelected(item);
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
		
		void setName(String name)
		{
			_personName = name;
		}
		boolean writeChanges()
		{
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			ContentValues cv = new ContentValues();
			cv.put("name", _personName);
			db.update("person", cv, "id = " + String.valueOf(_id), null);
			db.close();
			return true;
		}
		

	}
	
	class Currency {
		private int _id;
		private String _name;
		private String _shortName;
		private float _ratio;


		Currency(int id) {
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			Cursor cur = db.query("currency", null, "id = " + String.valueOf(id),
					null, null, null, null);
			if (cur.moveToFirst()) {
				int idColIndex = cur.getColumnIndex("id");
				int idNameIndex = cur.getColumnIndex("name");
				int idShortNameIndex = cur.getColumnIndex("shortname");
				int idUSDRatio = cur.getColumnIndex("usdratio");
				_id = cur.getInt(idColIndex);
				_name = cur.getString(idNameIndex);
				_shortName = cur.getString(idShortNameIndex);
				_ratio = cur.getFloat(idUSDRatio);
			} else {
				Log.d("db", "No records");
				_id = 0;
				_name = "";
				_shortName ="";
				_ratio = 0;
			}
			Log.d("New person", this.getName());
			Log.d("With id", String.valueOf(this.getId()));
			cur.close();
			db.close();
			//return _id;
		}
		int getId() {
			return _id;
		}
		String getName() {
			return _name;
		}
		void setName(String name)
		{
			_name = name;
		}
		String getShortName() {
			return _shortName;
		}
		void setShortName(String shortName)
		{
			_shortName = shortName;
		}
		float getRatio()
		{
			return _ratio;
		}
		void setRatio(float ratio)
		{
			_ratio = ratio;
		}
		boolean writeChanges()
		{
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			ContentValues cv = new ContentValues();
			cv.put("name", _name);
			cv.put("shortname", _shortName);
			cv.put("usdratio", _ratio);
			db.update("currency", cv, "id = " + String.valueOf(_id), null);
			db.close();
			return true;
		}

	}
}
