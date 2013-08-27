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
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.Toast;

public class addBuy extends Activity implements OnItemSelectedListener, OnClickListener {
	int type;
	int idNumer;
	DBHelper dbHelper;
	int trIndexStorage[];

	String peoples[];
	int peopleId[];
	
	stringAndId SAIcurrencies[];
	stringAndId SAIjourneys[];

	boolean init;
	ImplicitReceipt impR;
	/** Called when the activity is  created. */
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
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		init = false;
		dbHelper = new DBHelper(this);
		Intent intent = getIntent();
		type = intent.getIntExtra("typeOfAdd", 0);
	
		ActionBar actionBar;
		actionBar = getActionBar();
		actionBar.setDisplayHomeAsUpEnabled(true);

		Log.d(String.valueOf(type)," ");
		
		if(type == 8) 
			idNumer = intent.getIntExtra("id", 0);
		init  = true;
		// Toast.makeText(getApplicationContext(), String.valueOf(type),
		// Toast.LENGTH_SHORT).show();
		Spinner spnr;
		switch (type) {
		case 0:
			setContentView(R.layout.add_buy);
			spnr = (Spinner) findViewById(R.id.spinner3);
			spnr.setOnItemSelectedListener(this);
			Button btn = (Button) findViewById(R.id.button1);
			btn.setOnClickListener(this);
			actionBar.setTitle(R.string.labReceipts);
			getBuyValues();
			break;
		case 1:
			setContentView(R.layout.add_trip);
			btn = (Button) findViewById(R.id.button1);
			btn.setOnClickListener(this);
			/*
			Toast.makeText(getApplicationContext(), "Start get travellers",
					Toast.LENGTH_SHORT).show();*/
			actionBar.setTitle(R.string.labTrips);
			getTravellers();
			break;
		case 2:
			setContentView(R.layout.new_traveller);
			EditText txt = (EditText)findViewById(R.id.editText1);
			txt.clearFocus();
			actionBar.setTitle(R.string.labPersons);
			//txt.requestFocus();
			break;
		case 3:
			setContentView(R.layout.new_currency);
			actionBar.setTitle(R.string.labCurrences);
			break;
		case 8:
			setContentView(R.layout.add_buy);
			btn = (Button) findViewById(R.id.button1);
			btn.setOnClickListener(this);
			
			spnr = (Spinner) findViewById(R.id.spinner3);
			spnr.setOnItemSelectedListener(this);
			impR = new ImplicitReceipt(idNumer);
			getBuyValues();
			actionBar.setTitle(R.string.labReceipts);
			break;
		}
	}
	
	
	class ImplicitReceipt
	{
		int _id;
		String _description;
		float _value;
		int _whoBuy;
		String _whoUse;
		int _currency;
		int _travel;
		ImplicitReceipt(int position)
		{
			
			SQLiteDatabase db = dbHelper.getWritableDatabase();
			Cursor cur = db.query("buys", null, null, null, null, null, null);
			Log.d("ImplicitReceipt","Position is " + position);
			Log.d("Implicit Receipt", "In cursor is "+ cur.getCount() + " positions");
			if (cur.move(cur.getCount() - position)) 
			{
				int idColIndex = cur.getColumnIndex("id");
				int idDescriptionIndex = cur.getColumnIndex("desc");
				int idValueIndex = cur.getColumnIndex("value");
				int idCurrencyIndex = cur.getColumnIndex("currency");
				int idTravelIndex = cur.getColumnIndex("travel");
				int idWhoBuyIndex = cur.getColumnIndex("whobuy");
				int idWhoUseIndex = cur.getColumnIndex("whouse");
				
				_id = cur.getInt(idColIndex);
				_description = cur.getString(idDescriptionIndex);
				_value = cur.getFloat(idValueIndex);
				_whoBuy = cur.getInt(idWhoBuyIndex);
				_whoUse = cur.getString(idWhoUseIndex);
				_currency = cur.getInt(idCurrencyIndex);
			    _travel = cur.getInt(idTravelIndex);
			} else
				Log.d("dbimpr", "No records in curr");

			cur.close();
			db.close();
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
			if (!insertHelper(type)) return super.onOptionsItemSelected(item);
			Intent intent = new Intent();
			setResult(RESULT_OK, intent);
			finish();

			break;
		
		case R.id.item_reset:
			intent = new Intent();
			setResult(RESULT_OK, intent);
			finish();
			break;
		}
		return super.onOptionsItemSelected(item);
	}

	
	public boolean insertNameToPersons() {
		ContentValues cv = new ContentValues();
		SQLiteDatabase db = dbHelper.getWritableDatabase();

		EditText tvName = (EditText) findViewById(R.id.editText1);
		if(tvName.getText().toString().isEmpty())
		{
			Toast.makeText(getApplicationContext(), 
					getString(R.string.toast_fill_all_of_the_strings), Toast.LENGTH_SHORT).show();
			db.close();
			return false;
		}
		cv.put("name", tvName.getText().toString());

		db.insert("person", null, cv);

		db.close();
		return true;
	}

	public boolean insertDataToCurrency() {
		ContentValues cv = new ContentValues();
		SQLiteDatabase db = dbHelper.getWritableDatabase();

		EditText tvName = (EditText) findViewById(R.id.editText1);
		EditText tvShortName = (EditText) findViewById(R.id.editText2);
		EditText tvRatio = (EditText) findViewById(R.id.editText3);
		
		if (tvName.getText().toString().isEmpty()||tvShortName.getText().toString().isEmpty()
				||tvRatio.getText().toString().isEmpty())
		{
			Toast.makeText(getApplicationContext(), 
					getString(R.string.toast_fill_all_of_the_strings), Toast.LENGTH_SHORT).show();
			db.close();
			return false;
		}

		cv.put("name", tvName.getText().toString());
		cv.put("shortname", tvShortName.getText().toString());
		cv.put("usdratio", Float.parseFloat(tvRatio.getText().toString()));

		db.insert("currency", null, cv);
		//Log.d("dsfdsf", "New currency added " + String.valueOf(intid));

		db.close();
		return true;
	}

	public boolean insertHelper(int id) {
		switch (id) {
		case 0:
		return addNewBuy();
		case 1:
			return addNewTravel();
		case 2:
			return insertNameToPersons();
		case 3:
			return insertDataToCurrency();
		case 8:	
			/*SQLiteDatabase db = dbHelper.getReadableDatabase();
			db.delete("buys", "id = "+ impR._id, null);
			db.close();*/
			return addNewBuy(impR._id);
		}
		return false;
	}

	public void getBuyValues() {
		
		
		
		SAIcurrencies= fillSpinnerReverse("currency", "shortname", R.id.spinner1);
		
		SAIjourneys = fillSpinner("travels", "name", R.id.spinner3);
		if (type == 8)
		{
			Log.d("type is eight", " ");
		    EditText bufEtxt = (EditText) findViewById (R.id.editText1);
		    bufEtxt.setText(String.valueOf(impR._value));
		    bufEtxt = (EditText) findViewById (R.id.editText2);
		    bufEtxt.setText(impR._description);
			Spinner bufSpnr = (Spinner)findViewById(R.id.spinner1);	
			bufSpnr.setSelection(getPositionById (SAIcurrencies, impR._currency));
			bufSpnr = (Spinner)findViewById(R.id.spinner3);
			bufSpnr.setSelection(getPositionById (SAIjourneys, impR._travel));
		
		}
	}

	public void getTravellers() {
		SQLiteDatabase db = dbHelper.getWritableDatabase();

		Cursor cur = db.query("person", null, null, null, null, null, null);

		// Spinner spnr = (Spinner)findViewById(R.id.spinner1);
		ListView elvTravellers = (ListView) findViewById(R.id.listView1);
		elvTravellers.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);

		//Log.d("jhgjg", "start to work with db");

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
				
				//array_spinner1[i]+=":"+String.valueOf(trIndexStorage[i]);
				//array_spinner1[i] = String.valueOf(trIndexStorage[i]);
				//Log.d("dsfds", array_spinner1[i]);
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

	public boolean addNewTravel() {
		ContentValues cv = new ContentValues();
		SQLiteDatabase db = dbHelper.getWritableDatabase();

		EditText tvName = (EditText) findViewById(R.id.editText1);
		if (tvName.getText().toString().isEmpty())
		{
			Toast.makeText(getApplicationContext(), 
					getString(R.string.toast_fill_all_of_the_strings), Toast.LENGTH_SHORT).show();
			db.close();
            return false;	
		}
	
		cv.put("name", tvName.getText().toString());
		ListView lvTravellers = (ListView) findViewById(R.id.listView1);
		String buffer = new String();
		SparseBooleanArray sbArray = lvTravellers.getCheckedItemPositions();
		Log.d("There are cheked ", String.valueOf(sbArray.size()));

		boolean isFirst = true;
		for (int a = 0; a < sbArray.size(); a++) {
			if (sbArray.valueAt(a)){
				
				int i = sbArray.keyAt(a);
				Log.d("checked items" , String.valueOf(i));
				if (isFirst) 
					{
					buffer = " " + String.valueOf(trIndexStorage[i])+" ";
					isFirst = false;
					}
				else
				buffer += ", " + String.valueOf(trIndexStorage[i])+" ";
			}		
		}
		
		if (buffer.split(",").length < 2)
		{
			Toast.makeText(getApplicationContext(), 
					getString(R.string.select_at_least_two_people), Toast.LENGTH_SHORT).show();
			db.close();
            return false;	
		}

		
		Log.d("travellers", buffer+";");
		cv.put("travelers", buffer);

		db.insert("travels", null, cv);
		//Log.d("dsfdsf", "New trip added " + String.valueOf(intid));

		db.close();
return true;
	}

	public stringAndId [] fillSpinner(String table, String field, int spinId) {

		SQLiteDatabase db = dbHelper.getWritableDatabase();

		Cursor cur = db.query(table, null, null, null, null, null, null);
		
		stringAndId dataContainer[] = new stringAndId [cur.getCount()];

		Spinner spnr = (Spinner) findViewById(spinId);

		//Log.d("jhgjg", "start to work with db");

		if (cur.moveToLast()) {
			int idColIndex = cur.getColumnIndex("id");
			int idNameIndex = cur.getColumnIndex(field);
				//trIndexStorage = new int[cur.getCount()];
			// cur.getCount();
			String array_spinner1[];
			int i = 0;
			array_spinner1 = new String[cur.getCount()];
			do {
				dataContainer [i] = new stringAndId (cur.getInt(idColIndex),cur.getString(idNameIndex));
				array_spinner1[i] = cur.getString(idNameIndex);
				/*array_spinner1[i]+=":"+String.valueOf(dataContainer[i].getid());*/
				//Log.d("dsfds", array_spinner1[i]);
			    //dataContainer[i] = cur.getInt(idColIndex);
				//array_spinner1[i]+=String.valueOf(dataContainer[i].getid());
				i++;
			} while (cur.moveToPrevious());
			ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
					android.R.layout.simple_spinner_item, array_spinner1);
			adapter.setDropDownViewResource(android.R.layout.simple_dropdown_item_1line);
			spnr.setAdapter(adapter);
			spnr.setPrompt("hello");
		} else
			Log.d("db", "No records in curr");

		cur.close();
		db.close();
		return dataContainer;
	}
	
	public stringAndId [] fillSpinnerReverse(String table, String field, int spinId) {

		SQLiteDatabase db = dbHelper.getWritableDatabase();

		Cursor cur = db.query(table, null, null, null, null, null, null);
		
		stringAndId dataContainer[] = new stringAndId [cur.getCount()];

		Spinner spnr = (Spinner) findViewById(spinId);

		//Log.d("jhgjg", "start to work with db");

		if (cur.moveToFirst()) {
			int idColIndex = cur.getColumnIndex("id");
			int idNameIndex = cur.getColumnIndex(field);
				//trIndexStorage = new int[cur.getCount()];
			// cur.getCount();
			String array_spinner1[];
			int i = 0;
			array_spinner1 = new String[cur.getCount()];
			do {
				dataContainer [i] = new stringAndId (cur.getInt(idColIndex),cur.getString(idNameIndex));
				array_spinner1[i] = cur.getString(idNameIndex);
				/*array_spinner1[i]+=":"+String.valueOf(dataContainer[i].getid());*/
				//Log.d("dsfds", array_spinner1[i]);
			    //dataContainer[i] = cur.getInt(idColIndex);
				//array_spinner1[i]+=String.valueOf(dataContainer[i].getid());
				i++;
			} while (cur.moveToNext());
			ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
					android.R.layout.simple_spinner_item, array_spinner1);
			adapter.setDropDownViewResource(android.R.layout.simple_dropdown_item_1line);
			spnr.setAdapter(adapter);
			spnr.setPrompt("hello");
		} else
			Log.d("db", "No records in curr");

		cur.close();
		db.close();
		return dataContainer;
	}

	public String[] getAdapterStrById(String table, String field, String columns) {
		String rows[] = columns.split(",");
		
		for (int i = 0 ; i <rows.length; i++)
		{
			Log.d("rows to show", rows[i]);
			rows[i] = rows [i].trim();
		}
		
		String array_spinner1[];
		array_spinner1 = new String[rows.length];
		SQLiteDatabase db = dbHelper.getWritableDatabase();
		peopleId = new int[rows.length];

		Cursor cur;

		for (int j = 0; j < rows.length; j++) {
			Log.d("What we need to get", "id = " + rows[j]);
			Log.d("Show index ", String.valueOf(j));
			cur = db.query(table, null, "id = " + rows[j], null, null, null,
					null);
			if (cur.moveToLast()) {
				int idColIndex = cur.getColumnIndex("id");
				int idNameIndex = cur.getColumnIndex(field);
				
				//do {
					array_spinner1[j] = cur.getString(idNameIndex);
					peopleId[j] = cur.getInt(idColIndex);
					
					Log.d("we send to data", array_spinner1[j] + peopleId[j] + ":"+ j);
					
					//array_spinner1[j] += ":" + String.valueOf(peopleId[j]);
					
					//Log.d("dsfds", array_spinner1[j]);
				//} while (cur.moveToPrevious());
			}

		}
		
		for (int i = 0 ; i< array_spinner1.length; i++)
			Log.d("array spinner to send", array_spinner1[i]);

		return array_spinner1;
	}

	public String[] fillSpinner(String table, String field) {

		SQLiteDatabase db = dbHelper.getWritableDatabase();
		String array_spinner1[] = new String[1];
		Cursor cur = db.query(table, null, null, null, null, null, null);

		//Log.d("jhgjg", "start to work with db");

		if (cur.moveToLast()) {
			//int idColIndex = cur.getColumnIndex("id");
			int idNameIndex = cur.getColumnIndex(field);
			// cur.getCount();
			int i = 0;
			array_spinner1 = new String[cur.getCount()];
			do {
				array_spinner1[i] = cur.getString(idNameIndex);
				//Log.d("dsfds", array_spinner1[i]);
				i++;
			} while (cur.moveToPrevious());
		} else
			Log.d("db", "No records in curr");

		cur.close();
		db.close();

		return array_spinner1;
	}

	@Override
	public void onItemSelected(AdapterView<?> parentView,
			View selectedItemView, int position, long id) {

		Spinner spnr = (Spinner) findViewById(R.id.spinner2);
		
		Log.d("positions to show",fillSpinner("travels", "travelers")[position]);
		
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
				android.R.layout.simple_spinner_item, getAdapterStrById(
						"person", "name",
						fillSpinner("travels", "travelers")[position]));
		adapter.setDropDownViewResource(android.R.layout.simple_dropdown_item_1line);
		spnr.setAdapter(adapter);
		
		if ((type == 8)&&(init))
		{
			spnr.setSelection(returnPositionInList(impR._whoBuy));			
		}

		ListView lvView = (ListView) findViewById(R.id.listView1);
		ArrayAdapter<String> lvString = new ArrayAdapter<String>(this,
				android.R.layout.simple_list_item_multiple_choice,
				getAdapterStrById("person", "name",
						fillSpinner("travels", "travelers")[position]));
		lvString.setDropDownViewResource(android.R.layout.simple_list_item_multiple_choice);
		lvView.setAdapter(lvString);
		lvView.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);
		
		if ((type == 8)&&(init))
		{
			String [] whoUse = impR._whoUse.split(",");
			for (int i = 0 ; i < whoUse.length; i++)
			{
			lvView.setItemChecked(returnPositionInList(Integer.parseInt(whoUse[i].trim())), true);
			}
		init = false;
		}
	}

	@Override
	public void onNothingSelected(AdapterView<?> parentView) {
		Log.d("dsfasdf", "nothing selected");
	}
	
	private int returnPositionInList(int id)
	{
		for (int i = 0; i< peopleId.length; i++)
		{
		 if (peopleId[i] == id) return i;
		}
		return -1;

	}
	
	protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		outState.putBoolean("init", init);
		
		ListView lv1 = (ListView)findViewById(R.id.listView1);
		if (lv1 == null) return;
		
		SparseBooleanArray sb = lv1.getCheckedItemPositions();
		boolean [] chItems = new boolean [lv1.getCount()];
		
		for (int i = 0; i < sb.size(); i++)
		{
			if (sb.get(sb.keyAt(i))){
				chItems [sb.keyAt(i)] = true;
				Log.i("tag6", "Save true at position " + sb.keyAt(i));
			}
		}
		
		outState.putBooleanArray("checked_items", chItems);
	}
	
	protected void onRestoreInstanceState(Bundle savedInstanceState)
	{
		Log.i("tag6", "restoring");
		super.onRestoreInstanceState(savedInstanceState);
		init = savedInstanceState.getBoolean("init");
		
		
		//boolean [] state = savedInstanceState.getBooleanArray("checked_items");
		ListView lv1 = (ListView)findViewById(R.id.listView1);
		
		if (lv1 == null) return;
		Log.i("tag6", "List view is not null");
		/*for (int i = 0; i < state.length; i++)
		{
			lv1.setItemChecked(i, state[i]);
			Log.i("tag6", "Set true at position " + i);
		}*/
		
	}
	
	public boolean addNewBuy()
	{
		return addNewBuy(null);
	}
	public boolean addNewBuy(Integer id) {

		ContentValues cv = new ContentValues();
		SQLiteDatabase db = dbHelper.getWritableDatabase();

		EditText tvName = (EditText) findViewById(R.id.editText1);
		if (tvName.getText().toString().isEmpty())
		{
			Toast.makeText(getApplicationContext(), 
					getString(R.string.toast_fill_value_string), Toast.LENGTH_SHORT).show();
			db.close();
            return false;	
		}
		
		cv.put("value", Double.parseDouble(tvName.getText().toString()));
		Spinner spCurrency = (Spinner) findViewById(R.id.spinner1);
		cv.put("currency", SAIcurrencies[(int) spCurrency.getSelectedItemId()].getid());
		Spinner spTrip = (Spinner) findViewById(R.id.spinner3);
		cv.put("travel", SAIjourneys[(int) spTrip.getSelectedItemId()].getid());
		Spinner spWho = (Spinner) findViewById(R.id.spinner2);
		cv.put("whobuy", peopleId[(int) spWho.getSelectedItemId()]);
		cv.put("desc", ((EditText)findViewById(R.id.editText2)).getText().toString());

		ListView lvAffected = (ListView) findViewById(R.id.listView1);
		String buffer = new String();

		
		
		if (lvAffected.getCheckedItemCount() == 0) 
		{
			Toast.makeText(getApplicationContext(), 
					getString(R.string.toast_select_at_least_one_person), Toast.LENGTH_SHORT).show();
			db.close();
			return false;
		}
			SparseBooleanArray sbArray = lvAffected.getCheckedItemPositions();

			boolean isFirst = true;
			for (int i = 0; i < sbArray.size(); i++) {
				if (sbArray.valueAt(i))
				{
					if (isFirst)
					{
						buffer = " " + String.valueOf(peopleId[sbArray.keyAt(i)])+" ";
						isFirst = false;
					}
						else
					buffer += ", "+String.valueOf(peopleId[sbArray.keyAt(i)]) + " ";
				}
		}
			
			Log.d("tag","what is buffer");
			if (buffer.split(",").length == 0)
			{
				Log.d("tag", "We have to show toast");
				Toast.makeText(getApplicationContext(), 
						getString(R.string.select_at_least_two_people), Toast.LENGTH_SHORT).show();
				db.close();
	            return false;	
			}

			Log.d("Buffer is", buffer);
		cv.put("whouse", buffer);

		if (id == null)
		db.insert("buys", null, cv);
		else
			db.update("buys", cv, "id = " + id, null);
		//Log.d("dsfdsf", "New receipt added " + String.valueOf());
        //Toast.makeText(getApplicationContext(), "Buyer is "+ peopleId[(int) spWho.getSelectedItemId()], Toast.LENGTH_SHORT).show();
		db.close();
		return true;
	}

	class stringAndId {
		private int id;
		private String str;

		public stringAndId()
		{
			id = 0;
			str = "";
		}
		public stringAndId(int oid, String ostr) {
			id = oid;
			str = ostr;
		}

		public int getid() {
			return id;
		}

		public String getString() {
			return str;
		}
		public void setId(int eid)
		{
			id = eid;
		}
		public void setStr (String estr)
		{
			str = estr;
		}
	}
	
	static int getPositionById (stringAndId []sidArray, int id)
	{
		for (int i = 0; i < sidArray.length; i++)
		{
			if (sidArray[i].getid() == id) return i;
		}
		return -1;
		
	}

	class travelWithStr extends stringAndId {
		private String whoUse;
		private String [] people;
		private int people_out_list[];

		public travelWithStr(int oid, String ostr, String owhoUse) {
			super(oid, ostr);
			whoUse = owhoUse;
			//people = new ArrayList<String>();
			people = getPeopleFromString(owhoUse);
		}
		
		public String getWhoUse()
		{
			return whoUse;
		}
		
		public int getIndex(int i)
		{
			return people_out_list[i];
		}
		
		public String[] getPeople(){
			return people;
		}

		private String[] getPeopleFromString(String str) {
			String output_list[];
			String rows[] = str.split(",");
			
			for (int i = 0 ; i <rows.length; i++)
			{
				Log.d("rows to show", rows[i]);
				rows[i] = rows[i].trim();
			}
			
			output_list = new String[rows.length];
			SQLiteDatabase db = dbHelper.getWritableDatabase();
			people_out_list = new int[rows.length];
			Cursor cur;

			for (int j = 0; j < rows.length; j++) {
				//Log.d("sdfad", rows[j]);
				cur = db.query("person", null, "id = " + rows[j], null, null, null,
						null);
				if (cur.moveToLast()) {
					int idColIndex = cur.getColumnIndex("id");
					int idNameIndex = cur.getColumnIndex("name");
					do {
						output_list[j] = cur.getString(idNameIndex);
						people_out_list[j] = idColIndex;
						Log.d("N for ñ record", output_list[j] + String.valueOf(people_out_list[j]));
					} while (cur.moveToPrevious());
				}

			}
			return output_list;
		}

	}



	@Override
	public void onClick(View arg0) {
		ListView view = (ListView) findViewById(R.id.listView1);
		//Toast.makeText(this, "Select all", Toast.LENGTH_SHORT).show();
		for (int i = 0; i < view.getCount(); i++)
		view.setItemChecked(i, true);
	}

}
