package com.example.travelassist;

import java.util.ArrayList;

import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.example.travelassist.DBHelper.Tables;

public class ReceiptView extends AbstractFragmentActivity {

	/** Called when the activity is first created. */

	private String names(String who, SQLiteDatabase db) {
		String[] whoArray = who.split(",");
		String result = "";

		for (int i = 0; i < whoArray.length; i++) {
			int id = Integer.parseInt(whoArray[i].trim());
			result += AllTravellers.getInstance(db).getNameById(id) + ", ";
		}

		result = result.substring(0, result.length() - 2);
		return result;
	}

	@Override
	public Bundle settingsToBundle(int id, SQLiteDatabase db) {

		int[] ids = { R.id.textView1, R.id.textView5, R.id.textView2,
				R.id.textView4, R.id.textView7 };
		ArrayList<String> strings = new ArrayList<String>();

		Cursor cur = DBHelper.getCursor(Tables.BUYS, id, db);

		int valueId = cur.getColumnIndex("value");
		int descriptionId = cur.getColumnIndex("desc");
		int travelId = cur.getColumnIndex("travel");
		int currencyId = cur.getColumnIndex("currency");
		int whobuyId = cur.getColumnIndex("whobuy");
		int whouseId = cur.getColumnIndex("whouse");

		Log.i("tag6", "" + id);
		if (cur.moveToFirst()) {
			Log.i("tag6", "cur move to first");
			strings.add(""
					+ cur.getString(valueId)
					+ " "
					+ AllCurrencies.getInstance(db).getNameById(
							cur.getInt(currencyId)));
			strings.add(""
					+ AllTripNames.getInstance(db).getNameById(
							cur.getInt(travelId)));
			strings.add("" + cur.getString(descriptionId));
			strings.add(""
					+ AllTravellers.getInstance(db).getNameById(
							cur.getInt(whobuyId)));
			strings.add(names(cur.getString(whouseId), db));
		}
		cur.close();
		db.close();

		for (String a : strings)
			Log.i("tag6", a);
		Bundle bndl = new Bundle();
		bndl.putInt(ViewFragment.VIEW_ID_KEY, R.layout.show_buy);
		bndl.putIntArray(ViewFragment.ID_KEY, ids);
		bndl.putStringArrayList(ViewFragment.VALUE_KEY, strings);
		return bndl;
	}

	@Override
	Tables getTable() {
		return Tables.BUYS;
	}

	@Override
	String getTitleField() {
		return "desc";
	}

	@Override
	protected boolean itemDelete(int id, SQLiteDatabase db) {
		return DBHelper.removeItem(db, Tables.BUYS, id);
	}

	@Override
	protected void itemAdd() {
		Intent intent = new Intent(this, addBuy.class);
		intent.putExtra("typeOfAdd", 0);
		startActivityForResult(intent, 0);

	}

	@Override
	protected void itemEdit(int id) {
		Intent intent = new Intent(this, addBuy.class);
		/*
		 * Log.i("tab7","position "+getPosition(id, allIds));
		 * Log.i("tab7","all "+allIds.length);
		 */

		intent.putExtra("id", allIds.length - 1 - curPosPos);
		intent.putExtra("typeOfAdd", 8);
		startActivityForResult(intent, 0);
	}

	@Override
	protected void showProblems() {
		Toast.makeText(this,
				this.getResources().getStringArray(R.array.delete_problems)[0],
				Toast.LENGTH_SHORT).show();
	}

	@Override
	protected String getTitleFrag() {
		Log.i("tab7", getString(R.string.labReceipts));
		return getTitle().toString();
	}
}
