package com.example.travelassist;

import java.util.ArrayList;

import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.widget.Toast;

import com.example.travelassist.DBHelper.Tables;

public class ShowTraveller extends AbstractFragmentActivity {

	public Bundle settingsToBundle(int id, SQLiteDatabase db) {
		Cursor cur = DBHelper.getCursor(Tables.PERSONS, id, db);
		int nameId = cur.getColumnIndex("name");
		int[] ids = { R.id.textView1, R.id.textView6, R.id.textView2,
				R.id.textView7 };
		ArrayList<String> strings = new ArrayList<String>();

		if (cur.moveToFirst()) {
			strings.add(cur.getString(nameId));
		}
		cur.close();

		AllCurrencies curre = AllCurrencies.getInstance(db);
		String basicCurName = curre.getNameById(1);

		cur = DBHelper.getCursor(Tables.BUYS,
				"whouse LIKE \"% " + id + " %\" ", db);
		strings.add("" + cur.getCount());
		cur.close();
		cur = DBHelper.getCursor(Tables.BUYS, "whobuy = " + id, db);
		strings.add("" + cur.getCount());

		float amount = 0;
		int valId = cur.getColumnIndex("value");
		if (cur.moveToFirst()) {
			amount += cur.getFloat(valId);
		}

		strings.add("" + amount + " " + basicCurName);
		cur.close();
		db.close();
		Bundle bndl = new Bundle();
		bndl.putInt(ViewFragment.VIEW_ID_KEY, R.layout.fragment_person);
		bndl.putIntArray(ViewFragment.ID_KEY, ids);
		bndl.putStringArrayList(ViewFragment.VALUE_KEY, strings);
		return bndl;
	}

	@Override
	Tables getTable() {
		return Tables.PERSONS;
	}

	@Override
	String getTitleField() {
		return "name";
	}

	@Override
	protected boolean itemDelete(int id, SQLiteDatabase db) {
		return DBHelper.removeItem(db, Tables.TRIPS, "travelers", "" + id,
				Tables.PERSONS, id);
	}

	@Override
	protected void itemAdd() {

		Intent intent = new Intent(this, addBuy.class);
		intent.putExtra("typeOfAdd", 2);
		startActivityForResult(intent, 0);

	}

	@Override
	protected void itemEdit(int id) {
		Intent intent = new Intent(this, changePersonAndCurrency.class);
		//intent.putExtra("id", allIds.length - 1 - curPosPos);
		intent.putExtra("id", curPosPos);
		intent.putExtra("isPerson", true);
		startActivityForResult(intent, 0);

	}

	@Override
	protected void showProblems() {
		Toast.makeText(this,
				this.getResources().getStringArray(R.array.delete_problems)[2],
				Toast.LENGTH_SHORT).show();

	}

	@Override
	protected String getTitleFrag() {
		// TODO Auto-generated method stub
		//return getString(R.string.labPersons);
		return getTitle().toString();
	}

}
