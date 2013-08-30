package ru.optimal_drive.receipt_calculator;

import java.util.Stack;

import ru.optimal_drive.receipt_calculator.R;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.FragmentTransaction;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.app.NavUtils;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public abstract class AbstractFragmentActivity extends FragmentActivity
		implements ActionBar.TabListener, ItemRemovable {
	int id, nextId, prevId, count;

	int curPosId;
	int curPosPos;
	
	Stack<Integer> viewStack = new Stack<Integer> ();

	int[] allIds;
	String[] names;
	DBHelper dbHelper;
	SectionsPagerAdapter mSectionsPagerAdapter;
	ViewPager mViewPager;
	ActionBar actionBar;
	FragmentManager mFragmentManager;

	/** Called when the activity is first created. */
	@Override
	public final void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Intent intent = getIntent();
		dbHelper = new DBHelper(this);
		SQLiteDatabase db = dbHelper.getReadableDatabase();
		id = intent.getIntExtra("id", 0);
		Cursor cur = DBHelper.getCursor(getTable(), db);
		allIds = new int[cur.getCount()];
		names = new String[cur.getCount()];

		Log.i("tag6", "There are items: " + cur.getCount());
		if (cur.moveToLast()) {
			int idIndex = cur.getColumnIndex("id");
			int nameIndex = cur.getColumnIndex(this.getTitleField());
			int i = 0;
			do {
				allIds[i] = cur.getInt(idIndex);
				names[i] = cur.getString(nameIndex);
				Log.i("tab7", ""+allIds[i]+":"+names[i]);
				i++;
			} while (cur.moveToPrevious());
			count = i;
		}

		Log.i("tag6", "" + count);

		actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
		actionBar.setDisplayHomeAsUpEnabled(true);
		
		Log.i("tab7", "set new title in carousel");
		
		//actionBar.setTitle(getTitleField());
		
		setAdapter();
		mViewPager.setCurrentItem(getPosition(this.id, this.allIds));
		curPosId = this.id;		
	
	}
	
	@Override
	public void onBackPressed()
	{
		if (viewStack.isEmpty()) finish();
		else
			{
			int pos = viewStack.pop();
			
		mViewPager.setCurrentItem(pos);
		viewStack.pop();
		curPosId = pos;		
			}
	}
	
	
	private void setAdapter()
	{
		mFragmentManager = getSupportFragmentManager();
		
		mSectionsPagerAdapter = new SectionsPagerAdapter(
				getSupportFragmentManager());

		// Set up the ViewPager with the sections adapter.
		mViewPager = (ViewPager) findViewById(R.id.pager);

		mViewPager.setAdapter(mSectionsPagerAdapter);

		// When swiping between different sections, select the corresponding
		// tab. We can also use ActionBar.Tab#select() to do this if we have
		// a reference to the Tab.
		mViewPager
				.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
					@Override
					public void onPageSelected(int position) {
						// posChanged(position);
						actionBar.setSelectedNavigationItem(position);
						curPosId = allIds[position];
						curPosPos = position;
						Log.i("tab7", "current id is " + curPosId);
					}
				});

		// For each of the sections in the app, add a tab to the action bar.
		for (int i = 0; i < mSectionsPagerAdapter.getCount(); i++) {
			// Create a tab with text corresponding to the page title defined by
			// the adapter. Also specify this Activity object, which implements
			// the TabListener interface, as the callback (listener) for when
			// this tab is selected.
			actionBar.addTab(actionBar.newTab()
					.setText(mSectionsPagerAdapter.getPageTitle(i))
					.setTabListener(this));
		}

		}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.menu_for_view, menu);
		return true;
	}

	int getPosition(int id, int[] ids) {
		for (int i = 0; i < ids.length; i++) {
			if (ids[i] == id)
				return i;
		}
		throw new RuntimeException("No such id");
	}

	public abstract Bundle settingsToBundle(int id, SQLiteDatabase db);

	abstract DBHelper.Tables getTable();

	abstract String getTitleField();

	abstract protected boolean itemDelete(int id, SQLiteDatabase db);

	abstract protected void itemAdd();

	abstract protected void itemEdit(int id);

	abstract protected void showProblems();
	
	abstract protected String getTitleFrag();
	@Override
	public void onTabReselected(Tab arg0, FragmentTransaction arg1) {

	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {

		Log.i("tab7", "Current position id is " + curPosId);

		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			break;
		case R.id.item_add:
			itemAdd();
			break;
		case R.id.item_edit:
			Log.i("tab7", "" + curPosId);
			itemEdit(curPosId);
			break;
		case R.id.item_remove:
			RemoveConfirmationDialogFragment newFragment = new RemoveConfirmationDialogFragment();
			newFragment.id = curPosId;
			newFragment.setElement(this);
			newFragment.show(getSupportFragmentManager(), "dsfs");

			break;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onTabSelected(Tab tab, FragmentTransaction arg1) {

		mViewPager.setCurrentItem(tab.getPosition());
	}

	@Override
	protected void onActivityResult(int a, int b, Intent c) {
		//setAdapter();
		//mViewPager.setCurrentItem(getPosition(curPosId, this.allIds));
		//curPosId = this.id;
		Fragment topFragment = mFragmentManager.findFragmentByTag(mSectionsPagerAdapter.getActiveFragmentTag(mViewPager, curPosPos));
		Log.d("tab7", "We have fragment with pointer to "+ topFragment.toString());
		SQLiteDatabase db = dbHelper.getReadableDatabase();
		((ViewFragment)topFragment).setUpdateData(settingsToBundle(curPosId, db));
		((ViewFragment)topFragment).update();
		
		db = dbHelper.getReadableDatabase();
		Cursor cur = DBHelper.getCursor(getTable(), curPosId, db);
		cur.moveToFirst();
		int nameIndex = cur.getColumnIndex(this.getTitleField());
		names[curPosPos] = cur.getString(nameIndex);
		
		actionBar.getTabAt(curPosPos).setText(names[curPosPos]);
		
		db.close();
		
	}

	@Override
	public void onTabUnselected(Tab arg0, FragmentTransaction arg1) {
		
		viewStack.push(arg0.getPosition());
	}

	public class SectionsPagerAdapter extends FragmentPagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}
		
		public String getActiveFragmentTag (ViewPager container, int position)
		{
			String name = makeFragmentName(container.getId(), position);
			return name;
		}
		
		private String makeFragmentName (int viewId, int index)
		{
			return "android:switcher:" + viewId + ":" + index; 
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a DummySectionFragment (defined as a static inner class
			// below) with the page number as its lone argument.
			Fragment fragment = new ViewFragment();
			SQLiteDatabase db = dbHelper.getReadableDatabase();
			Log.i("tag6", "creation of new fragment with settingsToBundle");
			fragment.setArguments(settingsToBundle(allIds[position], db));
			db.close();
			return fragment;
		}

		@Override
		public int getCount() {
			// Log.d("tag6", ""+count);
			return count;
		}

		@Override
		public CharSequence getPageTitle(int position) {
			if (position < names.length)
				return names[position];
			return null;
		}
	}

	@Override
	public void remove(int id, int position) {
		SQLiteDatabase db = dbHelper.getWritableDatabase();
		if (itemDelete(id, db)) {
			db.close();
			finish();
		} else
			showProblems();
		db.close();

	}

	public static class RemoveConfirmationDialogFragment extends DialogFragment
			implements OnClickListener {
		private ItemRemovable element;
		int id;
		int position;

		public void setElement(ItemRemovable element) {
			this.element = element;
		}

		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			// LayoutInflater inflater = getActivity().getLayoutInflater();
			builder.setMessage(R.string.remove_for_notify_text)
					.setTitle(R.string.remove_for_notify)
					.setPositiveButton(R.string.strOk, this)
					.setNegativeButton("Cancel", null);
			return builder.create();
		}

		@Override
		public void onClick(DialogInterface arg0, int arg1) {
			// TODO Auto-generated method stub
			element.remove(id, position);
		}

	}

}
