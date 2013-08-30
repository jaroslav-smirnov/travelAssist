package ru.optimal_drive.receipt_calculator;

import java.util.Locale;
import java.util.Stack;

import ru.optimal_drive.receipt_calculator.DBHelper.Tables;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.FragmentTransaction;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Looper;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;

import ru.optimal_drive.receipt_calculator.R;
import com.google.ads.AdRequest;
import com.google.ads.AdView;

public class MainActivity extends FragmentActivity implements
		ActionBar.TabListener, OnItemClickListener, ItemRemovable {

	/**
	 * The {@link android.support.v4.view.PagerAdapter} that will provide
	 * fragments for each of the sections. We use a
	 * {@link android.support.v4.app.FragmentPagerAdapter} derivative, which
	 * will keep every loaded fragment in memory. If this becomes too memory
	 * intensive, it may be best to switch to a
	 * {@link android.support.v4.app.FragmentStatePagerAdapter}.
	 */
	SectionsPagerAdapter mSectionsPagerAdapter;

	Stack<Integer> viewStack = new Stack<Integer>();

	private static MainActivity activityInstance;
	private static int tabPosition = 0;
	public static final int TABS_NUMB = 4;
	private static DBHelper dbHelper;
	private SharedPreferences sPref;
	private static boolean isItFirst;

	private static int useCount = 0;

	private boolean getIsFirst() {
		sPref = getPreferences(MODE_PRIVATE);
		Log.d("Is it first", "" + sPref.getBoolean("is_first", true));
		useCount = sPref.getInt("used_x_times", 0);
		return sPref.getBoolean("is_first", true);
	}

	private void setNoFirst() {
		sPref = getPreferences(MODE_PRIVATE);
		Editor ed = sPref.edit();
		ed.putBoolean("is_first", false);
		ed.putInt("used_x_times", ++useCount);
		ed.commit();
		Log.d("Is it first", "" + sPref.getBoolean("is_first", true));

	}

	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;

	boolean deleteNow;
	boolean editNow;
	ActionBar actionBar;
	private static AdView adView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// final ActionBar actionBar = getActionBar();
		actionBar = getActionBar();

		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
		actionBar.setDisplayShowTitleEnabled(false);
		actionBar.setDisplayUseLogoEnabled(false);
		actionBar.setDisplayShowHomeEnabled(false);
		actionBar.setBackgroundDrawable(new ColorDrawable(R.color.orange));

		dbHelper = new DBHelper(this);

		isItFirst = getIsFirst();

		if (isItFirst) {
			DialogFragment newFragment = new FirstStartDialogFragment();
			newFragment.setCancelable(false);
			newFragment.show(getSupportFragmentManager(), "lalala");
		}

		// tabPosition = 0;
		activityInstance = this;
		// Set up the action bar.// Create the adapter that will return a
		// fragment for each of the three
		// primary sections of the app.
		Log.d("tag", "New section pager adapter");
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
						posChanged(position);
						actionBar.setSelectedNavigationItem(position);
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
		SQLiteDatabase db = dbHelper.getReadableDatabase();
		mViewPager.setCurrentItem(getStartPosition(db));

		db.close();
	}

	public static void posChanged(int position) {
		tabPosition = position;
		Log.i("tag3", "position is " + position);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.item_add: {
			editNow = false;
			deleteNow = false;
			actionBar.setDisplayShowTitleEnabled(false);
			if (!helpActions(tabPosition))
				return false;
			Intent intent = new Intent(this, addBuy.class);
			intent.putExtra("typeOfAdd", tabPosition);
			startActivityForResult(intent, 0);

		}
			return true;
		case R.id.item_edit: {
			editNow = true;
			deleteNow = false;
			actionBar.setDisplayShowTitleEnabled(true);
			actionBar.setTitle(R.string.select_item_to_edit_label);
			Toast.makeText(this, R.string.select_item_to_edit_label,
					Toast.LENGTH_SHORT).show();
		}
			return true;
		case R.id.item_remove: {
			deleteNow = true;
			editNow = false;
			actionBar.setDisplayShowTitleEnabled(true);
			actionBar.setTitle(R.string.select_item_to_remove_label);
			Toast.makeText(this, R.string.select_item_to_remove_label,
					Toast.LENGTH_SHORT).show();
		}
			return true;
		case R.id.item_help: {
			actionBar.setDisplayShowTitleEnabled(false);
			editNow = false;
			deleteNow = false;
			Intent intent = new Intent(this, HelpWindowClass.class);
			startActivity(intent);

		}
			return true;
		default:
			return super.onOptionsItemSelected(item);
		}

	}

	@Override
	protected void onStop() {
		this.setNoFirst();
		super.onStop();
	}

	@Override
	protected void onActivityResult(int RequestCode, int ResultCode, Intent data) {
		deleteNow = false;
		editNow = false;
		updateView();
		SQLiteDatabase db = dbHelper.getReadableDatabase();
		int posToAdd = -1;
		if (useCount < 5) posToAdd = getStartPosition(db);
		if (posToAdd > -1)
			mViewPager.setCurrentItem(posToAdd);
		db.close();
		if (RequestCode == 15) {

		}

	}

	@Override
	public void onBackPressed() {

		if (deleteNow || editNow) {
			deleteNow = false;
			editNow = false;
			actionBar.setDisplayShowTitleEnabled(false);
		} else {

			if (!viewStack.isEmpty()) {
				int pos = viewStack.pop();

				mViewPager.setCurrentItem(pos);
				viewStack.pop();
			} else {
				new AlertDialog.Builder(this)
						.setIcon(android.R.drawable.ic_dialog_alert)
						.setTitle(R.string.str_quit_label)
						.setMessage(R.string.str_are_you_sure)
						.setPositiveButton(R.string.yes,
								new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog,
											int which) {
										finish();
									}

								}).setNegativeButton(R.string.no, null).show();

			}
		}

	}

	void updateView() {
		SQLiteDatabase db = dbHelper.getReadableDatabase();

		Tables[] tbl = new Tables[] { Tables.BUYS, Tables.TRIPS,
				Tables.PERSONS, Tables.CURRENCIES };

		Fragment curFrag = getSupportFragmentManager().findFragmentByTag(
				mSectionsPagerAdapter.getActiveFragmentTag(mViewPager,
						tabPosition));
		View fragmentView = curFrag.getView();
		ListView lst = (ListView) fragmentView.findViewById(R.id.listView1);
		lst.setAdapter(DBHelper.getAdapter(this, db, tbl[tabPosition]));
		db.close();
	}

	@Override
	public void onTabSelected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
		if (useCount < 5)
			helpActions(tab.getPosition());
		mViewPager.setCurrentItem(tab.getPosition());
	}

	private boolean helpActions(int pos) {
		//int prev = pos == 0 ? 3 : pos - 1;
		int next = pos == 3 ? 0 : pos + 1;

		Tables[] tbl = new Tables[] { Tables.BUYS, Tables.TRIPS,
				Tables.PERSONS, Tables.CURRENCIES };

		SQLiteDatabase db = dbHelper.getReadableDatabase();
		int count = DBHelper.getCursor(tbl[next], db).getCount();
		
		db.close();
		if ((count == 0) && (pos == 0)) {
			Toast.makeText(this, R.string.str_add_trips, Toast.LENGTH_SHORT)
					.show();
			return false;
		}
		if ((count < 2) && (pos == 1)) {
			Toast.makeText(this, R.string.str_add_people, Toast.LENGTH_SHORT)
					.show();
			return false;
		}

		return true;
	}

	public static MainActivity getMainActivity() {
		if (activityInstance == null)
			throw new NullPointerException("No instance of object");
		return activityInstance;
	}

	@Override
	public void onTabUnselected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
		viewStack.push(tab.getPosition());

	}

	@Override
	public void onTabReselected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
	}

	private static Fragment[] fragmentArray = new Fragment[TABS_NUMB];

	// private int fragmentCount;

	/**
	 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
	 * one of the sections/tabs/pages.
	 */
	public class SectionsPagerAdapter extends FragmentPagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		public String getActiveFragmentTag(ViewPager container, int position) {
			String name = makeFragmentName(container.getId(), position);
			return name;
		}

		private String makeFragmentName(int viewId, int index) {
			return "android:switcher:" + viewId + ":" + index;
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a DummySectionFragment (defined as a static inner class
			// below) with the page number as its lone argument.
			Fragment fragment = new DummySectionFragment();
			// ((DummySectionFragment) fragment).setPosition(position);
			Bundle args = new Bundle();
			args.putInt(DummySectionFragment.ARG_SECTION_NUMBER, position + 1);
			fragment.setArguments(args);

			return fragment;
		}

		@Override
		public int getCount() {
			return TABS_NUMB;
		}

		@Override
		public CharSequence getPageTitle(int position) {
			Locale l = Locale.getDefault();
			switch (position) {
			case 0:
				return getString(R.string.labReceipts).toUpperCase(l);
			case 1:
				return getString(R.string.labTrips).toUpperCase(l);
			case 2:
				return getString(R.string.labPersons).toUpperCase(l);
			case 3:
				return getString(R.string.labCurrences).toUpperCase(l);

			}
			return null;
		}
	}

	/**
	 * A dummy fragment representing a section of the app, but that simply
	 * displays dummy text.
	 */
	public static class DummySectionFragment extends Fragment {
		/**
		 * The fragment argument representing the section number for this
		 * fragment.
		 */
		public static final String ARG_SECTION_NUMBER = "section_number";

		int fragmentPosition;

		public DummySectionFragment() {
		}

		@Override
		public void onCreate(Bundle state) {
			Bundle args = getArguments();
			fragmentPosition = args.getInt(ARG_SECTION_NUMBER) - 1;
			super.onCreate(state);

		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main_dummy,
					container, false);
			ListView dummyListView = (ListView) rootView
					.findViewById(R.id.listView1);
			MainActivity mainActivity = getMainActivity();
			dummyListView
					.setOnItemClickListener((OnItemClickListener) mainActivity);

			adView = (AdView) rootView.findViewById(R.id.adView);

			new Thread() {
				public void run() {
					Looper.prepare();
					adView.loadAd(new AdRequest()
							.addTestDevice(AdRequest.TEST_EMULATOR));
				}
			}.start();

			SQLiteDatabase db = dbHelper.getReadableDatabase();

			Tables[] tbl = new Tables[] { Tables.BUYS, Tables.TRIPS,
					Tables.PERSONS, Tables.CURRENCIES };

			dummyListView.setAdapter(DBHelper.getAdapter(getActivity(), db,
					tbl[fragmentPosition]));

			db.close();
			fragmentArray[fragmentPosition] = this;
			return rootView;
		}
	}

	static boolean sureToDel;

	@Override
	public void onItemClick(AdapterView<?> arg0, View arg1, int position,
			long arg3) {
		ListAdapter adpr = ((ListView) arg0).getAdapter();
		int id = ((ExSimpleAdapter) adpr).getIdByPos(position);
		
		if (deleteNow) {

			RemoveConfirmationDialogFragment newFragment = new RemoveConfirmationDialogFragment();
			newFragment.id = id;
			newFragment.position = tabPosition;
			newFragment.setElement(this);
			newFragment.show(getSupportFragmentManager(), "dsfs");

		} else if (editNow) {
			editPosition(tabPosition, position);
			editNow = false;
			actionBar.setDisplayShowTitleEnabled(false);
		} else {
			switch (tabPosition) {
			case 0: {
				Intent intent = new Intent(this, ReceiptView.class);
				intent.putExtra("id", id);
				startActivityForResult(intent, 0);
				// startActivity(intent);
				break;
			}
			case 1: {
				Intent intent = new Intent(this, ShowTravel.class);
				intent.putExtra("travelId", position);
				startActivityForResult(intent, 0);
				// startActivity(intent);
				break;
			}
			case 2: {
				Intent intent = new Intent(this, ShowTraveller.class);
				intent.putExtra("id", id);
				startActivityForResult(intent, 0);
				// startActivity(intent);
				break;
			}

			case 3: {
				Intent intent = new Intent(this, ShowCurrency.class);
				intent.putExtra("id", id);
				startActivityForResult(intent, 0);
				// startActivity(intent);
				break;
			}
			}
		}

	}

	private void editPosition(int pos, int posInList) {
		switch (pos) {
		case 0: {
			Intent intent = new Intent(this, addBuy.class);
			intent.putExtra("id", posInList);
			intent.putExtra("typeOfAdd", 8);
			startActivityForResult(intent, 0);

		}
			break;
		case 1: {
			Intent intent = new Intent(this, changeTravel.class);
			intent.putExtra("id", posInList);
			startActivityForResult(intent, 0);
			break;
		}
		case 2: {
			Intent intent = new Intent(this, changePersonAndCurrency.class);
			intent.putExtra("id", posInList);
			intent.putExtra("isPerson", true);
			startActivityForResult(intent, 0);

		}
			break;
		case 3: {
			{
				Intent intent = new Intent(this, changePersonAndCurrency.class);
				intent.putExtra("id", posInList);
				intent.putExtra("isPerson", false);
				startActivityForResult(intent, 0);
			}
		}
			break;
		}
	}

	private static boolean deletePosition(int pos, int id, SQLiteDatabase db) {
		switch (pos) {
		case 0: {
			return DBHelper.removeItem(db, Tables.BUYS, id);
		}
		case 1: {
			return DBHelper.removeItem(db, Tables.BUYS, "travel", id,
					Tables.TRIPS, id);

		}
		case 2: {
			return DBHelper.removeItem(db, Tables.TRIPS, "travelers", "" + id,
					Tables.PERSONS, id);
		}
		case 3: {
			if (id == 1)
				return false;
			return DBHelper.removeItem(db, Tables.BUYS, "currency", id,
					Tables.CURRENCIES, id);

		}
		}
		return false;
	}

	public static class FirstStartDialogFragment extends DialogFragment
			implements Button.OnClickListener {

		EditText shortName;
		EditText longName;
		Button btn;

		@Override
		public Dialog onCreateDialog(Bundle savedInstanceState) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			LayoutInflater inflater = getActivity().getLayoutInflater();
			View rootView = inflater.inflate(R.layout.first_start_layout, null);
			shortName = (EditText) rootView.findViewById(R.id.editText2);
			longName = (EditText) rootView.findViewById(R.id.editText1);
			btn = (Button) rootView.findViewById(R.id.button1);
			btn.setOnClickListener(this);
			builder.setView(rootView).setTitle(R.string.str_insert_basic_value);

			return builder.create();
		}

		@Override
		public void onClick(View v) {
			if ((shortName.length() == 0) || (longName.length() == 0)) {
				Toast.makeText(getActivity(), R.string.str_fill_fields,
						Toast.LENGTH_SHORT).show();
				return;
			}

			SQLiteDatabase db = dbHelper.getReadableDatabase();
			ContentValues cv = new ContentValues();
			cv.put("name", longName.getText().toString());
			cv.put("shortname", shortName.getText().toString());
			cv.put("usdratio", 1f);
			db.insert("currency", null, cv);
			db.close();
			isItFirst = false;

			this.dismiss();
		}
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
			//
			element.remove(id, position);
		}

	}

	public abstract static class TutorialDialogFragment extends DialogFragment
			implements View.OnClickListener {
		int id;
		int position;

		String _message;
		String _title;
		boolean isItLast;
		boolean contEnabled;

		public void addTitle(String title) {
			this._title = title;
		}

		public void addMessage(String message) {
			this._message = message;
		}

		public void setLast() {
			isItLast = true;
		}

		public void setContinueEnabled() {
			contEnabled = true;
		}

		@Override
		public final Dialog onCreateDialog(Bundle savedInstanceState) {
			AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
			LayoutInflater inflater = getActivity().getLayoutInflater();
			View rootView = inflater.inflate(R.layout.first_start_layout, null);
			Button btn1 = (Button) rootView.findViewById(R.id.button1);
			Button btn2 = (Button) rootView.findViewById(R.id.button2);

			btn1.setOnClickListener(this);
			btn2.setOnClickListener(this);

			builder.setMessage(_message).setTitle(_title);
			btn1.setText(R.string.str_add);
			if (!isItLast)
				btn2.setText(R.string.str_continue);
			else
				btn2.setText(R.string.str_finish);

			if (!contEnabled)
				btn2.setEnabled(false);

			return builder.create();
		}

		abstract void button1Click();

		abstract void button2Click();

		@Override
		public void onClick(View arg0) {
			switch (arg0.getId()) {
			case R.id.button1: {
				button1Click();
				break;
			}
			case R.id.button2: {
				button2Click();
				break;
			}
			}

		}

	}

	private int getStartPosition(SQLiteDatabase db) {
		Cursor cur = DBHelper.getCursor(Tables.PERSONS, db);
		if (cur.getCount() < 2) {
			new AlertDialog.Builder(this)
					.setIcon(android.R.drawable.ic_dialog_alert)
					.setTitle(R.string.first_st_not_caption)
					.setMessage(R.string.first_st_people)
					.setPositiveButton(R.string.strOk, null).show();
			return 2;
		} else {
			cur = DBHelper.getCursor(Tables.TRIPS, db);
			if (cur.getCount() == 0) {
				new AlertDialog.Builder(this)
						.setIcon(android.R.drawable.ic_dialog_alert)
						.setTitle(R.string.first_st_not_caption)
						.setMessage(R.string.first_st_travel)
						.setPositiveButton(R.string.strOk, null).show();
				return 1;
			} else {
				cur = DBHelper.getCursor(Tables.BUYS, db);
				if (cur.getCount() == 0) {
					new AlertDialog.Builder(this)
							.setIcon(android.R.drawable.ic_dialog_alert)
							.setTitle(R.string.first_st_not_caption)
							.setMessage(R.string.first_st_receipts)
							.setPositiveButton(R.string.strOk, null).show();
					return 0;
				} else {
					cur = DBHelper.getCursor(Tables.BUYS, db);
					if (cur.getCount() == 2) {
						new AlertDialog.Builder(this)
								.setIcon(android.R.drawable.ic_dialog_alert)
								.setTitle(R.string.first_st_view_caption)
								.setMessage(R.string.first_st_view_calculation)
								.setPositiveButton(R.string.strOk, null).show();
						return 1;
					}
				}
			}
		}
		return -1;
	}

	@Override
	public void remove(int id, int position) {

		SQLiteDatabase db = dbHelper.getWritableDatabase();
		if (deletePosition(position, id, db)) {
			updateView();
			deleteNow = false;
			actionBar.setDisplayShowTitleEnabled(false);
		} else
			Toast.makeText(
					this,
					this.getResources().getStringArray(R.array.delete_problems)[position],
					Toast.LENGTH_SHORT).show();
		db.close();

	}
}
