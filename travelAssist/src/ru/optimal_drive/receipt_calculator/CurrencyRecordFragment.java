package ru.optimal_drive.receipt_calculator;

import ru.optimal_drive.receipt_calculator.R;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class CurrencyRecordFragment extends AbstractRecordFragment {
	
	@Override
	public View onCreateView(LayoutInflater inflater,
			ViewGroup container, Bundle savedInstanceState){
		View rootView = inflater.inflate(R.layout.fragment_currency,
				container, false);
		
		
		
		return rootView;
		
	}


}
