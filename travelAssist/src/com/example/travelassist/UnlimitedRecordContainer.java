package com.example.travelassist;

import android.database.sqlite.SQLiteDatabase;

import com.example.travelassist.DBHelper.Tables;

public abstract class UnlimitedRecordContainer extends AbstractRecordContainer {
	UnlimitedRecordContainer(int id, Tables table, SQLiteDatabase db)
	{
		super.setTable(table);
		int [] nIds = DBHelper.getNeighborId(id, table, db);
		super.setId(id, nIds[0], nIds[1]);
	}
	
	

}
