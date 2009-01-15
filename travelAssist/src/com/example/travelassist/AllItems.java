package com.example.travelassist;

import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import com.example.travelassist.DBHelper.Tables;

abstract class AllItems {
	
	
	private int id[];
	private String name[];
	public final int size;
	
	protected AllItems(SQLiteDatabase db, Tables table, String fieldName)
	{
		Cursor cur = DBHelper.getCursor(table, db);
		size = cur.getCount();
		id = new int[size];
		name = new String[size];
		int i = 0;
		if (cur.moveToLast()) {
			int idColIndex = cur.getColumnIndex("id");
			int idNameIndex = cur.getColumnIndex(fieldName);
			do {
				Log.d("tag", ""+i);
				id[i] = cur.getInt(idColIndex);
				name[i++] = cur.getString(idNameIndex);
				} while (cur.moveToPrevious());
		} else
			{
			//throw new RuntimeException("No records in table currencies");
			}

		cur.close();

		
	}
	
	
	
	
	//public static abstract AllItems update (SQLiteDatabase db);
	
	@Override 
	public String toString()
	{
		StringBuilder builder = new StringBuilder();
		for (int i = 0; i < size; i++)
		{
			builder.append(""+id[i]+" : "+name[i]);
		}
		return builder.toString();
	}
	
	public int getPosById(int id)
	{
		for (int i = 0; i < size; i++)
			if (this.id[i] == id) return i;
		
		throw new RuntimeException("No element with id = " + id);
	}
	
	public String getNameById(int id)
	{
		return name[getPosById(id)];
	}
	
	public String getNameByPos(int pos)
	{
		return name[pos];
	}
	
}
