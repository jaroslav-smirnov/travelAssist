package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;

import android.database.sqlite.SQLiteDatabase;

public class AllCurrencies extends AllItems {
	
	private static AllCurrencies instancePointer;
	
	public AllCurrencies(SQLiteDatabase db)
	{super(db, Tables.CURRENCIES, "shortname");
	}

	public static AllCurrencies update(SQLiteDatabase db) {
		instancePointer =  new AllCurrencies(db);
		
		return instancePointer;
	}
	
	public static AllCurrencies getInstance(SQLiteDatabase db)
	{
		if (instancePointer == null) return new AllCurrencies(db);
		else return instancePointer;
	}
}
