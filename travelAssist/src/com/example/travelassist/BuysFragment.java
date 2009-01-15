package com.example.travelassist;

import android.database.Cursor;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.example.travelassist.DBHelper.Tables;

public class BuysFragment extends AbstractRecordFragment {

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View rootView = inflater.inflate(R.layout.show_buy, container, false);
		Cursor cur = DBHelper.getCursor(Tables.BUYS, id, db);
		if (cur.getCount() == 0)
			throw new RuntimeException("No buy record with id = " + id);
		Toast.makeText(getActivity(), "" + this.id, Toast.LENGTH_SHORT).show();

		/*TextView valueAndCur = (TextView) rootView.findViewById(R.id.textView1);
		TextView tripName = (TextView) rootView.findViewById(R.id.textView2);
		TextView description = (TextView) rootView.findViewById(R.id.textView3);
		TextView whoBuy = (TextView) rootView.findViewById(R.id.textView5);
		TextView whoUse = (TextView) rootView.findViewById(R.id.textView7);
/*
		valueAndCur.setText(""
				+ cur.getFloat(cur.getColumnIndex("value"))
				+ DBHelper.getStringByIndex(db, cur, Types.CLOB,
						cur.getColumnIndex("currency")));
		tripName.setText(DBHelper.getStringByIndex(db, cur, Types.VARCHAR,
						cur.getColumnIndex("travel")));
		description.setText(cur.getString(cur.getColumnIndex("desc")));
		whoBuy.setText(DBHelper.getStringByIndex(db, cur, Types.BIT,
						cur.getColumnIndex("whobuy")));
		String [] numbs = cur.getString(cur.getColumnIndex("whouse")).split(",");
		String whoUseStr = "";
		for (int i = 0; i < numbs.length; i++)
		{
			whoUseStr+= 
		}
		*/
		//whouse

		return rootView;
	}

}
