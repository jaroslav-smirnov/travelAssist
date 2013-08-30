package ru.optimal_drive.receipt_calculator;

import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public abstract class AbstractRecordFragment extends Fragment {

	int id;
	SQLiteDatabase db;
	 
	public void setIdAndDB(int id, SQLiteDatabase db)
	{
		Bundle settingsBundle = new Bundle();
		settingsBundle.putInt("id", id);
		this.db = db;
		this.setArguments(settingsBundle);
	}
	
	@Override
	public void setArguments(Bundle bndl)
	{
		if (bndl.containsKey("id")) this.id = bndl.getInt("id");
		super.setArguments(bndl);
	}

	
	@Override
	public abstract View onCreateView(LayoutInflater inflater,
			ViewGroup container, Bundle savedInstanceState);

}
