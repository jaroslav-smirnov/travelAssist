package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;
import android.database.Cursor;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import ru.optimal_drive.receipt_calculator.R;

public class PersonFragment extends AbstractRecordFragment {
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View rootView = inflater.inflate(R.layout.show_buy, container, false);
		Cursor cur = DBHelper.getCursor(Tables.BUYS, id, db);
		if (cur.getCount() == 0)
			throw new RuntimeException("No buy record with id = " + id);
		Toast.makeText(getActivity(), "" + this.id, Toast.LENGTH_SHORT).show();

		/*TextView personName = (TextView) rootView.findViewById(R.id.textView1);
		TextView perosnInfo = (TextView) rootView.findViewById(R.id.textView2);*/
		return rootView;
	}

}
