package tw.com.prlific.pl2303gmodemstatus;

//import tw.com.prolific.pl2303hxdgpio.R;


import java.io.IOException;
import java.util.Random;

import tw.com.prolific.driver.pl2303g.PL2303GDriver;

import android.app.Activity;
import android.content.Context;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.text.format.Time;
import android.util.Log;
import android.util.TypedValue;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.RadioButton;
import android.widget.RadioGroup;



public class PL2303GModemStatusActivity extends Activity {

	// debug settings

	private static final boolean SHOW_DEBUG = true;

	PL2303GDriver mSerial;

	String TAG = "PL2303G_APLog";

	private int mTextFontSize = 12;

	// BaudRate.B4800, DataBits.D8, StopBits.S1, Parity.NONE, FlowControl.RTSCTS
	private PL2303GDriver.BaudRate mBaudrate = PL2303GDriver.BaudRate.B9600;


	private ScrollView mSvText;
	private StringBuilder msbStrContent = new StringBuilder();
	private TextView mTvResponse; // response

	private Button mbOpen,mbGetModemStatus; 

	private RadioGroup mRadioGroup_DTR, mRadioGroup_RTS;
	private RadioButton mRadionBtn_DTR_On,mRadionBtn_DTR_Off, mRadionBtn_RTS_On, mRadionBtn_RTS_Off;

	private ImageView mImageDSR_OnOff,mImageRI_OnOff,mImageDCD_OnOff,mImageCTS_OnOff; 


	private static final String ACTION_USB_PERMISSION = "tw.com.prolific.pl2303gmodemstatus.USB_PERMISSION";


	@Override
	protected void onCreate(Bundle savedInstanceState) {

		Log.d(TAG, "Enter onCreate");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_pl2303_hxdmodem_status);


		mbOpen = (Button) findViewById(R.id.btnOpen);
		mbOpen.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				openUsbSerial();
			}
		});

		mbGetModemStatus = (Button) findViewById(R.id.btnGetModemStatus);
		mbGetModemStatus.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				PL2303G_GetModemStauts();
			}
		});

		mRadionBtn_DTR_On = (RadioButton) findViewById(R.id.radioDTR_ON);
		mRadionBtn_DTR_Off = (RadioButton) findViewById(R.id.radioDTR_Off);

		mRadioGroup_DTR = (RadioGroup)findViewById(R.id.radioGroupDTR);
		mRadioGroup_DTR.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				// TODO Auto-generated method stub

				PL2303HXD_DTR_SignalChange(group, checkedId);
			}
		});	


		mRadionBtn_RTS_On = (RadioButton) findViewById(R.id.radioRTS_ON);
		mRadionBtn_RTS_Off = (RadioButton) findViewById(R.id.radioRTS_Off);

		mRadioGroup_RTS = (RadioGroup)findViewById(R.id.radioGroupRTS);
		mRadioGroup_RTS.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				// TODO Auto-generated method stub

				PL2303HXD_RTS_SignalChange(group, checkedId);
			}
		});	


		mImageDSR_OnOff= (ImageView) findViewById(R.id.imageDSR);
		mImageDSR_OnOff.setImageResource(R.drawable.ic_off1);

		mImageRI_OnOff= (ImageView) findViewById(R.id.imageRI);
		mImageRI_OnOff.setImageResource(R.drawable.ic_off1);

		mImageDCD_OnOff= (ImageView) findViewById(R.id.imageDCD);
		mImageDCD_OnOff.setImageResource(R.drawable.ic_off1);

		mImageCTS_OnOff= (ImageView) findViewById(R.id.imageCTS);
		mImageCTS_OnOff.setImageResource(R.drawable.ic_off1);


		mSvText = (ScrollView) findViewById(R.id.svText);
		msbStrContent.setLength(0);
		mSvText.fullScroll(ScrollView.FOCUS_DOWN);


		mTvResponse = (TextView) findViewById(R.id.mTvResponse);
		mTvResponse.setTextSize(TypedValue.COMPLEX_UNIT_PT, mTextFontSize);
		mTvResponse.setText("");

		// get service
		mSerial = new PL2303GDriver(
				(UsbManager) getSystemService(Context.USB_SERVICE), this,
				ACTION_USB_PERMISSION);

		// check USB host function.
		if (!mSerial.PL2303USBFeatureSupported()) {

			Toast.makeText(this, "No Support USB host API", Toast.LENGTH_SHORT)
			.show();

			mTvResponse.setText("No Support USB host API");
			Log.d(TAG, "No Support USB host API");

			mSerial = null;
		}

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.pl2303_hxdmodem_status, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	protected void onStop() {
		Log.d(TAG, "Enter onStop");
		super.onStop();
		Log.d(TAG, "Leave onStop");
	}

	@Override
	protected void onDestroy() {
		Log.d(TAG, "Enter onDestroy");
		if (mSerial != null) {
			mSerial.end();
			mSerial = null;
			
		}
		super.onDestroy();
		Log.d(TAG, "Leave onDestroy");
	}

	public void onStart() {
		Log.d(TAG, "Enter onStart");
		super.onStart();
		Log.d(TAG, "Leave onStart");
	}

	@Override
	public void onResume() {
		Log.d(TAG, "Enter onResume");
		super.onResume();
		String action = getIntent().getAction();
		Log.d(TAG, "onResume:" + action);

		if (!mSerial.isConnected()) {
			if (SHOW_DEBUG) {
				Log.d(TAG, "New instance : " + mSerial);
			}

			if (!mSerial.enumerate()) {

				Toast.makeText(this, "no more devices found",
						Toast.LENGTH_SHORT).show();
				return;
			} else {
				Log.d(TAG, "onResume:enumerate succeeded!");
			}
		}// if isConnected
		Toast.makeText(this, "attached", Toast.LENGTH_SHORT).show();
		mTvResponse.setText("");

		Log.d(TAG, "Leave onResume");
	}
	// open PL2303G port
	private void openUsbSerial() {
		Log.d(TAG, "Enter  openUsbSerial");

		if (null == mSerial) {
			Log.d(TAG, "mSerial is null");
			// get service
			mSerial = new PL2303GDriver(
					(UsbManager) getSystemService(Context.USB_SERVICE), this,
					ACTION_USB_PERMISSION);

			if (!mSerial.enumerate()) {

				Toast.makeText(this, "no more devices found",
						Toast.LENGTH_SHORT).show();
				return;
			} else {

				Log.d(TAG, "enumerate succeeded!");
			}

		}

		if (mSerial.isConnected()) {
			if (SHOW_DEBUG) {
				Log.d(TAG, "openUsbSerial : isConnected ");
			}

			mBaudrate = PL2303GDriver.BaudRate.B115200;

			if (!mSerial.InitByBaudRate(mBaudrate)) {
				if(!mSerial.PL2303Device_IsHasPermission()) {
					Toast.makeText(this, "cannot open, maybe no permission", Toast.LENGTH_SHORT).show();
					msbStrContent.append("cannot open, maybe no permission/n");
					mTvResponse.setText("cannot open, maybe no permission");
				}

				if(mSerial.PL2303Device_IsHasPermission() && (!mSerial.PL2303Device_IsSupportChip())) {
					Toast.makeText(this, "cannot open, maybe this chip has no support, please use PL2303G chip.", Toast.LENGTH_SHORT).show();
					msbStrContent.append("cannot open, maybe this chip has no support, please use PL2303G chip./n");
					mTvResponse.setText("cannot open, maybe this chip has no support, please use PL2303G chip");
				}

			} else {
				Toast.makeText(this, "connected", Toast.LENGTH_SHORT).show();
				msbStrContent.append("connected/n");
				mTvResponse.setText("connected");
				
			}
		}// isConnected

		Log.d(TAG, "Leave openUsbSerial");
		msbStrContent.append("Leave openUsbSerial/n");

		// mTvResponse.setText(msbStrContent);

	}// openUsbSerial




	private void PL2303HXD_DTR_SignalChange(RadioGroup group, int checkedId){

		int res;
		Log.d(TAG, "Enter  PL2303HXD_DTR_SignalChange");

		if(null==mSerial)
			return;   	 

		switch(checkedId){
		case R.id.radioDTR_ON:

			res= mSerial.setDTR (true);

			if(res<0) {
				Log.d(TAG, "Fail to setDTR");
				return;
			}

			Log.d(TAG, "DTR ON");
			mTvResponse.setText(mRadionBtn_DTR_On.getText());

			break;

		case R.id.radioDTR_Off:

			res= mSerial.setDTR (false);

			if(res<0) {
				Log.d(TAG, "Fail to setDTR");
				return;
			}

			Log.d(TAG, "DTR Off");
			mTvResponse.setText(mRadionBtn_DTR_Off.getText());

			break;

		default:
			Log.d(TAG, "no change ");
			break;
		}

		Log.d(TAG, "Exit  PL2303HXD_DTR_SignalChange");
	}

	private void PL2303HXD_RTS_SignalChange(RadioGroup group, int checkedId){

		int res;
		Log.d(TAG, "Enter  PL2303HXD_RTS_SignalChange");

		if(null==mSerial)
			return;   	 

		switch(checkedId){
		case R.id.radioRTS_ON:

			res= mSerial.setRTS (true);

			if(res<0) {
				Log.d(TAG, "Fail to setRTS");
				return;
			}

			Log.d(TAG, "RTS ON");
			mTvResponse.setText(mRadionBtn_RTS_On.getText());

			break;

		case R.id.radioRTS_Off:

			res= mSerial.setRTS (false);

			if(res<0) {
				Log.d(TAG, "Fail to setRTS");
				return;
			}

			Log.d(TAG, "RTS Off");
			mTvResponse.setText(mRadionBtn_RTS_Off.getText());

			break;

		default:
			Log.d(TAG, "no change ");
			break;
		}

		Log.d(TAG, "Exit  PL2303HXD_RTS_SignalChange");

	}

	private void PL2303G_GetModemStauts(){
		int[] res = new int[2];
		int Status=0;

		StringBuilder sb = new StringBuilder();

		

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;
		
		Log.d(TAG, "Enter  PL2303G_GetModemStauts");

		res=mSerial.PL2303G_GetCommModemStatus();

		if (res[0]<0 ) {
			Log.d(TAG, "PL2303G_GetCommModemStatus: failed");
			sb.append("PL2303G_GetCommModemStatus: failed\n");
			return;
		} else {
			Status= res[1];
			Log.d(TAG, "PL2303G_GetCommModemStatus :" + Status);
			sb.append("PL2303G_GetCommModemStatus :" + Status +"\n");
		}

		//CTS
		if((Status & PL2303GDriver.PL2303G_CTS_ON)==PL2303GDriver.PL2303G_CTS_ON) {
			Log.d(TAG, "PL2303G_CTS_ON");
			sb.append("PL2303G_CTS_ON)\n");
			mImageCTS_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_on));
		}
		else{

			Log.d(TAG, "PL2303G_CTS_Off");
			sb.append("PL2303G_CTS_Off\n");	
			mImageCTS_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_off1));
		}

		//DCD
		if((Status & PL2303GDriver.PL2303G_DCD_ON)==PL2303GDriver.PL2303G_DCD_ON) {
			Log.d(TAG, "PL2303G_DCD_ON");
			sb.append("PL2303G_DCD_ON\n");
			mImageDCD_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_on));
		}
		else{

			Log.d(TAG, "PL2303G_DCD_Off");
			sb.append("PL2303G_DCD_Off\n");	
			mImageDCD_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_off1));
		}

		//DSR
		if((Status & PL2303GDriver.PL2303G_DSR_ON)==PL2303GDriver.PL2303G_DSR_ON) {
			Log.d(TAG, "PL2303G_DSR_ON");
			sb.append("PL2303G_DSR_ON\n");			
			mImageDSR_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_on));
		}
		else{

			Log.d(TAG, "PL2303G_DSR_Off");
			sb.append("PL2303G_DSR_Off\n");
			mImageDSR_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_off1));
		}

		//RI
		if((Status & PL2303GDriver.PL2303G_RI_ON)==PL2303GDriver.PL2303G_RI_ON) {
			Log.d(TAG, "PL2303G_RI_ON");
			sb.append("PL2303G_RI_ON\n");
			mImageRI_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_on));
			
		}
		else{

			Log.d(TAG, "PL2303G_RI_Off");
			sb.append("PL2303G_RI_Off\n");
			mImageRI_OnOff.setImageDrawable(getResources().getDrawable(R.drawable.ic_off1));
		}

		try {
			mTvResponse.setText(sb.toString());
		} catch (Exception e) {
			Log.d(TAG, "TestModemStauts error:::"); 
			e.printStackTrace();
		}


		Log.d(TAG, "Leave  PL2303G_GetModemStauts"); 
	}

}
