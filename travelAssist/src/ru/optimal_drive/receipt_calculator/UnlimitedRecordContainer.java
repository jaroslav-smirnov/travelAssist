package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;
import android.database.sqlite.SQLiteDatabase;


public abstract class UnlimitedRecordContainer extends AbstractRecordContainer {
	UnlimitedRecordContainer(int id, Tables table, SQLiteDatabase db)
	{
		super.setTable(table);
		int [] nIds = DBHelper.getNeighborId(id, table, db);
		super.setId(id, nIds[0], nIds[1]);
	}
	
	

}
