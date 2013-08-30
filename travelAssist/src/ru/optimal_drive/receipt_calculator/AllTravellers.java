package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;
import android.database.sqlite.SQLiteDatabase;


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
