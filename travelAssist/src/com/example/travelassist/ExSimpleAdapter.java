package com.example.travelassist;

import java.util.List;
import java.util.Map;

import android.content.Context;
import android.widget.SimpleAdapter;

public class ExSimpleAdapter extends SimpleAdapter {
	
	private int [] tableId;
	ExSimpleAdapter(Context ctx, List<? extends Map<String,?>> data, int resource, String[] from, int [] to, int [] ids)
	{
		super (ctx, data, resource, from, to);
		tableId = ids;
	}
	public int getIdByPos(int pos){
	return tableId[pos];
	}
}
