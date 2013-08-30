package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;
import android.database.sqlite.SQLiteDatabase;


public class BuyRecordContainer extends UnlimitedRecordContainer  {
	
	BuyRecordContainer(int id, SQLiteDatabase db)
	{
		super(id, Tables.BUYS, db);
		super.setFragment(null);
	}
	

	public BuyRecordContainer getNext(SQLiteDatabase db)
	{
		if (super.count == 0) return this;
		return new BuyRecordContainer (super.nextId, db);
	}
	public BuyRecordContainer getPrevious(SQLiteDatabase db)
	{
		if (super.count == 0) return this;
		return new BuyRecordContainer (super.prevId, db);		
	}
	public void getFragment(SQLiteDatabase db)
	{
		//setFragment(new BuysF)
	}
	

}
