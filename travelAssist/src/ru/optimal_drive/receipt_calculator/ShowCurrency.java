package ru.optimal_drive.receipt_calculator;

import java.util.ArrayList;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;

import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.widget.Toast;

import ru.optimal_drive.receipt_calculator.R;

public class ShowCurrency extends AbstractFragmentActivity {

	@Override
	public Bundle settingsToBundle(int id, SQLiteDatabase db) {
		int[] ids = { R.id.textView1, R.id.textView2, R.id.textView3,
				R.id.textView8 };
		ArrayList<String> strings = new ArrayList<String>();

		Cursor cur = DBHelper.getCursor(Tables.CURRENCIES, id, db);

		int shortNameId = cur.getColumnIndex("shortname");
		int nameId = cur.getColumnIndex("name");
		int usdRatioId = cur.getColumnIndex("usdratio");

		if (cur.moveToFirst()) {
			strings.add(cur.getString(nameId));
			strings.add(cur.getString(shortNameId));
			strings.add("" + cur.getFloat(usdRatioId));
		}
		cur.close();
		cur = DBHelper.getCursor(Tables.BUYS, "currency = " + id, db);
		strings.add("" + cur.getCount());
		Bundle bndl = new Bundle();
		bndl.putInt(ViewFragment.VIEW_ID_KEY, R.layout.fragment_currency);
		bndl.putIntArray(ViewFragment.ID_KEY, ids);
		bndl.putStringArrayList(ViewFragment.VALUE_KEY, strings);
		return bndl;

	}

	@Override
	Tables getTable() {
		return Tables.CURRENCIES;
	}

	@Override
	String getTitleField() {
		return "name";
	}

	@Override
	protected boolean itemDelete(int id, SQLiteDatabase db) {
		if (id == 1)
			return false;
		return DBHelper.removeItem(db, Tables.BUYS, "currency", id,
				Tables.CURRENCIES, id);
}

	@Override
	protected void itemAdd() {
		Intent intent = new Intent(this, addBuy.class);
		intent.putExtra("typeOfAdd", 3);
		startActivityForResult(intent, 0);
	}

	@Override
	protected void itemEdit(int id) {
		Intent intent = new Intent(this, changePersonAndCurrency.class);
		//intent.putExtra("id", (allIds.length - 1 - curPosPos));
		intent.putExtra("id", curPosPos);
		intent.putExtra("isPerson", false);
		startActivityForResult(intent, 0);
	}

	@Override
	protected void showProblems() {Toast.makeText(this,
			this.getResources().getStringArray(R.array.delete_problems)[3],
			Toast.LENGTH_SHORT).show();

	}

	@Override
	protected String getTitleFrag() {
		//return getString(R.string.labCurrences);
		return getTitle().toString();
	}

}
