package com.example.travelassist;

import android.database.sqlite.SQLiteDatabase;

import com.example.travelassist.DBHelper.Tables;

public class AllTravellers extends AllItems {

	private static AllTravellers instanceReference;

	protected AllTravellers(SQLiteDatabase db) {
		super(db, Tables.PERSONS, "name");
		// TODO Auto-generated constructor stub
	}

	
	public static AllTravellers update(SQLiteDatabase db) {
		// TODO Auto-generated method stub
		return new AllTravellers(db);
	}
	
	public static AllTravellers getInstance (SQLiteDatabase db)
	{
		if (instanceReference == null) return new AllTravellers(db);
		return instanceReference;
		
	}


}
