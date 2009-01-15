package com.example.travelassist;

import java.util.ArrayList;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;

public class ViewFragment extends Fragment {

	public final static String ID_KEY = "id_key";
	public final static String VALUE_KEY = "val_key";
	public final static String VIEW_ID_KEY = "view_id_key";
	public final static String LIST_VIEW_ADAPTER = "list_view_adapter";
	
	int[] ids;
	int viewId;
	ArrayList<String> values;
	SimpleExpandableListAdapter adapter;
	
	@Override
	public void onCreate(Bundle state) {
		Bundle args = getArguments();
		setUpdateData (args);
		super.onCreate(state);
	}
	
	public void setUpdateData (Bundle args)
	{
		ids = args.getIntArray(ID_KEY);
		viewId = args.getInt(VIEW_ID_KEY);
		values = args.getStringArrayList(VALUE_KEY);
		adapter = (SimpleExpandableListAdapter)args.get(LIST_VIEW_ADAPTER);
	}
	
	@Override
	public String toString()
	{
		String buffer = "my fragment is"+ values.toString() + viewId + ids;
		return buffer;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View rootView = inflater.inflate(viewId, container, false);
		for (int i = 0; i < ids.length; i++) {
			TextView tv1 = (TextView) rootView.findViewById(ids[i]);
			tv1.setText(values.get(i));
		}
		return rootView;
	}
	
	public void update()
	{
		View rootView = this.getView();
		Log.i("tab7", "Trying to update activity");
		for (int i = 0; i < ids.length; i++) {
			TextView tv1 = (TextView) rootView.findViewById(ids[i]);
			tv1.setText(values.get(i));
		}
	
	}
	
	
	
}


