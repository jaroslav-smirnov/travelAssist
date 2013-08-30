package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;
import android.database.sqlite.SQLiteDatabase;


class AllTripNames extends AllItems {
	
	private static AllTripNames instanceReference;

	protected AllTripNames(SQLiteDatabase db) {
		super(db, Tables.TRIPS, "name");
		// TODO Auto-generated constructor stub
	}

	public static AllTripNames update(SQLiteDatabase db) {
		// TODO Auto-generated method stub
		return new AllTripNames(db);
	}
	
	public static AllTripNames getInstance (SQLiteDatabase db)
	{
		if (instanceReference == null) return new AllTripNames(db);
		return instanceReference;
		
	}

}
