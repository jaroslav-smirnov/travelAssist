package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;
import android.database.sqlite.SQLiteDatabase;
import android.support.v4.app.Fragment;


public abstract class AbstractRecordContainer {

	private static int id;
	protected int prevId;
	protected int nextId;
	protected int count;
	protected Tables table;
	private Fragment curFragment;
	
	/*AbstractRecordContainer (int id, int prevId, int nextId, Tables table)
	{
		setId(id, prevId, nextId);
	}*/
	
	public static int getId()
	{
		return id;
	}
	public void setId(int id, int prevId, int nextId)
	{
		AbstractRecordContainer.id = id;
		this.prevId = prevId;
		this.nextId = nextId;
		count = Math.abs(prevId - nextId);
	}
	public void setTable (Tables table)
	{
	 this.table = table;
	}
	
	protected void setFragment (Fragment fragment)
	{
		this.curFragment = fragment;
	}
	

	
	
	public abstract AbstractRecordContainer getNext(SQLiteDatabase db);
	public abstract AbstractRecordContainer getPrevious(SQLiteDatabase db);
	
	public Fragment getFragment()
	{
		return curFragment;
	}
	
	
}
