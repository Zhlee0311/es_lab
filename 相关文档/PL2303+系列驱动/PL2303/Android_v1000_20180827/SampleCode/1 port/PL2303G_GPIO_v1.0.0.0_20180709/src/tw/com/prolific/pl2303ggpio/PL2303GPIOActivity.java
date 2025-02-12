package tw.com.prolific.pl2303ggpio;

//import com.prolific.pl2303hxdsimpletest.R;

import java.io.IOException;
import java.util.Arrays;
import java.util.Random;

//import tw.com.prolific.app.pl2303terminal.R;
//import tw.com.prolific.app.pl2303terminal.R;
import tw.com.prolific.driver.pl2303g.PL2303GDriver;
import tw.com.prolific.driver.pl2303g.PL2303GDriver.DataBits;
import tw.com.prolific.driver.pl2303g.PL2303GDriver.FlowControl;
import tw.com.prolific.driver.pl2303g.PL2303GDriver.Parity;
import tw.com.prolific.driver.pl2303g.PL2303GDriver.StopBits;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.text.format.Time;
import android.util.Log;
import android.util.TypedValue;
import android.view.Menu;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.CheckBox;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ScrollView;

public class PL2303GPIOActivity extends Activity {

	// debug settings
	// private static final boolean SHOW_DEBUG = false;

	private static final boolean SHOW_DEBUG = true;

	// Defines of Display Settings
	//private static final int DISP_CHAR = 0;


	PL2303GDriver mSerial;

	// private ScrollView mSvText;
	// private StringBuilder mText = new StringBuilder();

	String TAG = "PL2303G_APLog";

	private int mTextFontSize = 12;

	private Button mbtOpen; // open button
	private Button mbtClose; // close button

	// GPIO 0
	private CheckBox mcbGPIO_0; // GPIO_0 , output enable /disable
	private Button mbtGetGPIO_0; // Get GPIO_0 value
	private Button mbtSetGPIO_0; // Set GPIO_0 value
	private EditText metGPIO_0_val; // GPIO_0_value

	// GPIO 1
	private CheckBox mcbGPIO_1; // GPIO_1 , output enable /disable
	private Button mbtGetGPIO_1; // Get GPIO_1 value
	private Button mbtSetGPIO_1; // Set GPIO_1 value
	private EditText metGPIO_1_val; // GPIO_1_value

	// GPIO 2
	private CheckBox mcbGPIO_2; // GPIO_2 , output enable /disable
	private Button mbtGetGPIO_2; // Get GPIO_2 value
	private Button mbtSetGPIO_2; // Set GPIO_2 value
	private EditText metGPIO_2_val; // GPIO_2_value

	// GPIO 3
	private CheckBox mcbGPIO_3; // GPIO_3 , output enable /disable
	private Button mbtGetGPIO_3; // Get GPIO_3 value
	private Button mbtSetGPIO_3; // Set GPIO_3 value
	private EditText metGPIO_3_val; // GPIO_3_value

	// GPIO 4
	private CheckBox mcbGPIO_4; // GPIO_4 , output enable /disable
	private Button mbtGetGPIO_4; // Get GPIO_4 value
	private Button mbtSetGPIO_4; // Set GPIO_4 value
	private EditText metGPIO_4_val; // GPIO_4_value

	// GPIO 5
	private CheckBox mcbGPIO_5; // GPIO_5 , output enable /disable
	private Button mbtGetGPIO_5; // Get GPIO_5 value
	private Button mbtSetGPIO_5; // Set GPIO_5 value
	private EditText metGPIO_5_val; // GPIO_5_value

	// GPIO 6
	private CheckBox mcbGPIO_6; // GPIO_6 , output enable /disable
	private Button mbtGetGPIO_6; // Get GPIO_6 value
	private Button mbtSetGPIO_6; // Set GPIO_6 value
	private EditText metGPIO_6_val; // GPIO_6_value

	// GPIO 7
	private CheckBox mcbGPIO_7; // GPIO_7 , output enable /disable
	private Button mbtGetGPIO_7; // Get GPIO_7 value
	private Button mbtSetGPIO_7; // Set GPIO_7 value
	private EditText metGPIO_7_val; // GPIO_7_value

	private ScrollView mSvText;
	private StringBuilder msbStrContent = new StringBuilder();
	private TextView mTvResponse; // response

	// BaudRate.B4800, DataBits.D8, StopBits.S1, Parity.NONE, FlowControl.RTSCTS
	private PL2303GDriver.BaudRate mBaudrate = PL2303GDriver.BaudRate.B9600;
	private PL2303GDriver.DataBits mDataBits = PL2303GDriver.DataBits.D8;
	private PL2303GDriver.Parity mParity = PL2303GDriver.Parity.NONE;
	private PL2303GDriver.StopBits mStopBits = PL2303GDriver.StopBits.S1;
	private PL2303GDriver.FlowControl mFlowControl = PL2303GDriver.FlowControl.OFF;

	private static final String ACTION_USB_PERMISSION = "tw.com.prolific.pl2303ggpio.USB_PERMISSION";

	// Linefeed
	// private final static String BR = System.getProperty("line.separator");

	@Override
	public void onCreate(Bundle savedInstanceState) {

		Log.d(TAG, "Enter onCreate");

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_pl2303_gpio);

		mSvText = (ScrollView) findViewById(R.id.sVText);
		msbStrContent.setLength(0);
		mSvText.fullScroll(ScrollView.FOCUS_DOWN);

		mTvResponse = (TextView) findViewById(R.id.tVResponse);
		mTvResponse.setTextSize(TypedValue.COMPLEX_UNIT_PT, mTextFontSize);
		mTvResponse.setText("");

		// etGPIO_val = (EditText) findViewById(R.id.editText1);

		// open button
		mbtOpen = (Button) findViewById(R.id.btOpenPort);
		mbtOpen.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				openUsbSerial();
			}
		});

		// close button
		/*
		mbtClose = (Button) findViewById(R.id.btClosePort);
		mbtClose.setEnabled(false);
		mbtClose.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				closeUsbSerial();
			}
		});
		 */

		// ----- GPIO 0----
		// Enable / Disable GPIO 0
		mcbGPIO_0 = (CheckBox) findViewById(R.id.cBGPIO_0);
		mcbGPIO_0.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_0();
			}
		});

		// Get GPIO 0
		mbtGetGPIO_0 = (Button) findViewById(R.id.btGetGPIO_0);
		mbtGetGPIO_0.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_0_Val();
			}
		});

		// Set GPIO 0
		mbtSetGPIO_0 = (Button) findViewById(R.id.btSetGPIO_0);
		mbtSetGPIO_0.setEnabled(false);
		mbtSetGPIO_0.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_0_Val();
			}
		});

		// GPIO_0 value; 1 or 0.
		metGPIO_0_val = (EditText) findViewById(R.id.eTGPIOval_0);

		// -----GPIO 1-----
		// Enable / Disable GPIO 1
		mcbGPIO_1 = (CheckBox) findViewById(R.id.cBGPIO_1);
		mcbGPIO_1.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_1();
			}
		});

		// Get GPIO 1
		mbtGetGPIO_1 = (Button) findViewById(R.id.btGetGPIO_1);
		mbtGetGPIO_1.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_1_Val();
			}
		});

		// Set GPIO 1
		mbtSetGPIO_1 = (Button) findViewById(R.id.btSetGPIO_1);
		mbtSetGPIO_1.setEnabled(false);
		mbtSetGPIO_1.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_1_Val();
			}
		});

		// GPIO_1 value; 1 or 0.
		metGPIO_1_val = (EditText) findViewById(R.id.eTGPIOval_1);

		// ---- GPIO 2 ----
		// Enable / Disable GPIO 2
		mcbGPIO_2 = (CheckBox) findViewById(R.id.cBGPIO_2);
		//mcbGPIO_2.setChecked(true);
		mcbGPIO_2.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_2();
			}
		});

		// Get GPIO 2
		mbtGetGPIO_2 = (Button) findViewById(R.id.btGetGPIO_2);
		//mbtGetGPIO_2.setEnabled(false);
		mbtGetGPIO_2.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_2_Val();
			}
		});

		// Set GPIO 2
		mbtSetGPIO_2 = (Button) findViewById(R.id.btSetGPIO_2);
		mbtSetGPIO_2.setEnabled(false);
		mbtSetGPIO_2.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_2_Val();
			}
		});

		// GPIO_2 value; 1 or 0.
		metGPIO_2_val = (EditText) findViewById(R.id.eTGPIOval_2);

		// ---- GPIO 3 ----
		// Enable / Disable GPIO 3
		mcbGPIO_3 = (CheckBox) findViewById(R.id.cBGPIO_3);
		//mcbGPIO_3.setChecked(true);
		mcbGPIO_3.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_3();
			}
		});

		// Get GPIO 3
		mbtGetGPIO_3 = (Button) findViewById(R.id.btGetGPIO_3);
		//mbtGetGPIO_3.setEnabled(false);
		mbtGetGPIO_3.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_3_Val();
			}
		});

		// Set GPIO 3
		mbtSetGPIO_3 = (Button) findViewById(R.id.btSetGPIO_3);
		mbtSetGPIO_3.setEnabled(false);
		mbtSetGPIO_3.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_3_Val();
			}
		});

		// GPIO_3 value; 1 or 0.
		metGPIO_3_val = (EditText) findViewById(R.id.eTGPIOval_3);

		// ---- GPIO 4 ----
		// Enable / Disable GPIO 4
		mcbGPIO_4 = (CheckBox) findViewById(R.id.cBGPIO_4);
		mcbGPIO_4.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_4();
			}
		});

		// Get GPIO 4
		mbtGetGPIO_4 = (Button) findViewById(R.id.btGetGPIO_4);
		mbtGetGPIO_4.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_4_Val();
			}
		});

		// Set GPIO 4
		mbtSetGPIO_4 = (Button) findViewById(R.id.btSetGPIO_4);
		mbtSetGPIO_4.setEnabled(false);
		mbtSetGPIO_4.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_4_Val();
			}
		});

		// GPIO_4 value; 1 or 0.
		metGPIO_4_val = (EditText) findViewById(R.id.eTGPIOval_4);

		// ---- GPIO 5 ----
		// Enable / Disable GPIO 5
		mcbGPIO_5 = (CheckBox) findViewById(R.id.cBGPIO_5);
		mcbGPIO_5.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_5();
			}
		});

		// Get GPIO 5
		mbtGetGPIO_5 = (Button) findViewById(R.id.btGetGPIO_5);
		mbtGetGPIO_5.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_5_Val();
			}
		});

		// Set GPIO 5
		mbtSetGPIO_5 = (Button) findViewById(R.id.btSetGPIO_5);
		mbtSetGPIO_5.setEnabled(false);
		mbtSetGPIO_5.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_5_Val();
			}
		});

		// GPIO_5 value; 1 or 0.
		metGPIO_5_val = (EditText) findViewById(R.id.eTGPIOval_5);

		// ---- GPIO 6 ----
		// Enable / Disable GPIO 6
		mcbGPIO_6 = (CheckBox) findViewById(R.id.cBGPIO_6);
		mcbGPIO_6.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_6();
			}
		});

		// Get GPIO 6
		mbtGetGPIO_6 = (Button) findViewById(R.id.btGetGPIO_6);
		mbtGetGPIO_6.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_6_Val();
			}
		});

		// Set GPIO 6
		mbtSetGPIO_6 = (Button) findViewById(R.id.btSetGPIO_6);
		mbtSetGPIO_6.setEnabled(false);
		mbtSetGPIO_6.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_6_Val();
			}
		});

		// GPIO_6 value; 1 or 0.
		metGPIO_6_val = (EditText) findViewById(R.id.eTGPIOval_6);

		// ---- GPIO 7 ----
		// Enable / Disable GPIO 7
		mcbGPIO_7 = (CheckBox) findViewById(R.id.cBGPIO_7);
		mcbGPIO_7.setOnClickListener(new CheckBox.OnClickListener() {
			public void onClick(View v) {
				OutputEnable_GPIO_7();
			}
		});

		// Get GPIO 7
		mbtGetGPIO_7 = (Button) findViewById(R.id.btGetGPIO_7);
		mbtGetGPIO_7.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				GetGPIO_7_Val();
			}
		});

		// Set GPIO 7
		mbtSetGPIO_7 = (Button) findViewById(R.id.btSetGPIO_7);
		mbtSetGPIO_7.setEnabled(false);
		mbtSetGPIO_7.setOnClickListener(new Button.OnClickListener() {
			public void onClick(View v) {
				SetGPIO_7_Val();
			}
		});

		// GPIO_7 value; 1 or 0.
		metGPIO_7_val = (EditText) findViewById(R.id.eTGPIOval_7);

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

		// if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action))
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

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_pl2303_gpio, menu);
		return true;
	}

	// open PL2303HXD port
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
		// return;

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
					Toast.makeText(this, "cannot open, maybe this chip has no support, please use PL2303HXD / RA / EA chip.", Toast.LENGTH_SHORT).show();
					msbStrContent.append("cannot open, maybe this chip has no support, please use PL2303HXD / RA / EA chip./n");
					mTvResponse.setText("cannot open, maybe this chip has no support, please use PL2303HXD / RA / EA chip");
				}

				// mTvResponse.setText(msbStrContent);
				// mTvResponse.setText("cannot open, maybe no permission");

			} else {
				Toast.makeText(this, "connected", Toast.LENGTH_SHORT).show();
				msbStrContent.append("connected/n");
				mTvResponse.setText("connected");
				
				mSerial.PL2303G_GPIO_B_CNT_ENABLE(true);
				
				//mbtClose.setEnabled(true);
				//mbtOpen.setEnabled(false);

				// mTvResponse.setText(msbStrContent);

			}
		}// isConnected

		Log.d(TAG, "Leave openUsbSerial");
		msbStrContent.append("Leave openUsbSerial/n");

		// mTvResponse.setText(msbStrContent);

	}// openUsbSerial

	// close PL2303HXD port
	private void closeUsbSerial() {

		Log.d(TAG, "Enter  closeUsbSerial");

		if (mSerial != null) {
			mSerial.end();
			// mSerial = null;

			mbtClose.setEnabled(false);
			mbtOpen.setEnabled(true);
			mTvResponse.setText("");
		}

		Log.d(TAG, "Leave  closeUsbSerial");

	}

	// Output Enable / disable GPIO 0
	private void OutputEnable_GPIO_0() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_0");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_0.isChecked()) {

			mbtSetGPIO_0.setEnabled(true);
			mbtGetGPIO_0.setEnabled(false);
			metGPIO_0_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(0, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_0 optput enable: failed");
				mTvResponse.setText("GPIO_0 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_0 optput enable: OK");
				mTvResponse.setText("GPIO_0 optput enable: OK");
			}

		} else {
			mbtSetGPIO_0.setEnabled(false);
			mbtGetGPIO_0.setEnabled(true);
			metGPIO_0_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(0, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_0 optput disable: failed");
				mTvResponse.setText("GPIO_0 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_0 optput disable: OK");
				mTvResponse.setText("GPIO_0 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_0");
	}

	// Output Enable / disable GPIO 1
	private void OutputEnable_GPIO_1() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_1");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_1.isChecked()) {

			mbtSetGPIO_1.setEnabled(true);
			mbtGetGPIO_1.setEnabled(false);
			metGPIO_1_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(1, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_1 optput enable: failed");
				mTvResponse.setText("GPIO_1 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_1 optput enable: OK");
				mTvResponse.setText("GPIO_1 optput enable: OK");
			}

		}

		else {

			mbtSetGPIO_1.setEnabled(false);
			mbtGetGPIO_1.setEnabled(true);
			metGPIO_1_val.setEnabled(false);

			res = mSerial.PL2303G_Enable_GPIO_B(1, false);

			if (res < 0) {
				Log.d(TAG, "GPIO_1 optput disable: failed");
				mTvResponse.setText("GPIO_1 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_1 optput disable: OK");
				mTvResponse.setText("GPIO_1 optput disable: OK");
			}
		}

		Log.d(TAG, "Leave  OutputEnable_GPIO_1");
	}

	// Output Enable / disable GPIO 2
	private void OutputEnable_GPIO_2() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_2");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_2.isChecked()) {

			mbtSetGPIO_2.setEnabled(true);
			mbtGetGPIO_2.setEnabled(false);
			metGPIO_2_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(2, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_2 optput enable: failed");
				mTvResponse.setText("GPIO_2 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_2 optput enable: OK");
				mTvResponse.setText("GPIO_2 optput enable: OK");
			}

		} else {
			mbtSetGPIO_2.setEnabled(false);
			mbtGetGPIO_2.setEnabled(true);
			metGPIO_2_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(2, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_2 optput disable: failed");
				mTvResponse.setText("GPIO_2 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_2 optput disable: OK");
				mTvResponse.setText("GPIO_2 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_2");
	}

	// Output Enable / disable GPIO 3
	private void OutputEnable_GPIO_3() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_3");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_3.isChecked()) {

			mbtSetGPIO_3.setEnabled(true);
			mbtGetGPIO_3.setEnabled(false);
			metGPIO_3_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(3, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_3 optput enable: failed");
				mTvResponse.setText("GPIO_3 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_3 optput enable: OK");
				mTvResponse.setText("GPIO_3 optput enable: OK");
			}

		} else {
			mbtSetGPIO_3.setEnabled(false);
			mbtGetGPIO_3.setEnabled(true);
			metGPIO_3_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(3, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_3 optput disable: failed");
				mTvResponse.setText("GPIO_3 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_3 optput disable: OK");
				mTvResponse.setText("GPIO_3 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_3");
	}

	// Output Enable / disable GPIO 4
	private void OutputEnable_GPIO_4() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_4");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_4.isChecked()) {

			mbtSetGPIO_4.setEnabled(true);
			mbtGetGPIO_4.setEnabled(false);
			metGPIO_4_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(4, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_4 optput enable: failed");
				mTvResponse.setText("GPIO_4 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_4 optput enable: OK");
				mTvResponse.setText("GPIO_4 optput enable: OK");
			}

		} else {
			mbtSetGPIO_4.setEnabled(false);
			mbtGetGPIO_4.setEnabled(true);
			metGPIO_4_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(4, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_4 optput disable: failed");
				mTvResponse.setText("GPIO_4 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_4 optput disable: OK");
				mTvResponse.setText("GPIO_4 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_4");
	}

	// Output Enable / disable GPIO 5
	private void OutputEnable_GPIO_5() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_5");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_5.isChecked()) {

			mbtSetGPIO_5.setEnabled(true);
			mbtGetGPIO_5.setEnabled(false);
			metGPIO_5_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(5, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_5 optput enable: failed");
				mTvResponse.setText("GPIO_5 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_5 optput enable: OK");
				mTvResponse.setText("GPIO_5 optput enable: OK");
			}

		} else {
			mbtSetGPIO_5.setEnabled(false);
			mbtGetGPIO_5.setEnabled(true);
			metGPIO_5_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(5, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_5 optput disable: failed");
				mTvResponse.setText("GPIO_5 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_5 optput disable: OK");
				mTvResponse.setText("GPIO_5 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_5");
	}

	// Output Enable / disable GPIO 6
	private void OutputEnable_GPIO_6() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_6");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_6.isChecked()) {

			mbtSetGPIO_6.setEnabled(true);
			mbtGetGPIO_6.setEnabled(false);
			metGPIO_6_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(6, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_6 optput enable: failed");
				mTvResponse.setText("GPIO_6 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_6 optput enable: OK");
				mTvResponse.setText("GPIO_6 optput enable: OK");
			}

		} else {
			mbtSetGPIO_6.setEnabled(false);
			mbtGetGPIO_6.setEnabled(true);
			metGPIO_6_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(6, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_6 optput disable: failed");
				mTvResponse.setText("GPIO_6 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_6 optput disable: OK");
				mTvResponse.setText("GPIO_6 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_6");
	}

	// Output Enable / disable GPIO 7
	private void OutputEnable_GPIO_7() {
		int res;
		Log.d(TAG, "Enter  OutputEnable_GPIO_7");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		if (mcbGPIO_7.isChecked()) {

			mbtSetGPIO_7.setEnabled(true);
			mbtGetGPIO_7.setEnabled(false);
			metGPIO_7_val.setEnabled(true);

			res = mSerial.PL2303G_Enable_GPIO_B(7, true);

			if (res < 0) {
				Log.d(TAG, "GPIO_7 optput enable: failed");
				mTvResponse.setText("GPIO_7 optput enable: failed");
			} else {
				Log.d(TAG, "GPIO_7 optput enable: OK");
				mTvResponse.setText("GPIO_7 optput enable: OK");
			}

		} else {
			mbtSetGPIO_7.setEnabled(false);
			mbtGetGPIO_7.setEnabled(true);
			metGPIO_7_val.setEnabled(false);
			res = mSerial.PL2303G_Enable_GPIO_B(7, false);
			if (res < 0) {
				Log.d(TAG, "GPIO_7 optput disable: failed");
				mTvResponse.setText("GPIO_7 optput disable: failed");
			} else {
				Log.d(TAG, "GPIO_7 optput disable: OK");
				mTvResponse.setText("GPIO_7 optput disable: OK");
			}
		}
		Log.d(TAG, "Leave  OutputEnable_GPIO_7");
	}

	// Get GPIO_0 value
	private void GetGPIO_0_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_0_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(0);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_0_Val: failed");
			mTvResponse.setText("Get GPIO_0_Val: failed");
		} else {
			Log.d(TAG, "GPIO 0 :" + res[1]);
			mTvResponse.setText("GPIO 0 :" + res[1]);
		}
	}

	// Get GPIO_1 value
	private void GetGPIO_1_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_1_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(1);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_1_Val: failed");
			mTvResponse.setText("Get GPIO_1_Val: failed");
		} else {
			Log.d(TAG, "GPIO 1 :" + res[1]);
			mTvResponse.setText("GPIO 1 :" + res[1]);
		}
	}

	// Get GPIO_2 value
	private void GetGPIO_2_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_2_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(2);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_2_Val: failed");
			mTvResponse.setText("Get GPIO_2_Val: failed");
		} else {
			Log.d(TAG, "GPIO 2 :" + res[1]);
			mTvResponse.setText("GPIO 2 :" + res[1]);
		}
	}

	// Get GPIO_3 value
	private void GetGPIO_3_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_3_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(3);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_3_Val: failed");
			mTvResponse.setText("Get GPIO_3_Val: failed");
		} else {
			Log.d(TAG, "GPIO 3 :" + res[1]);
			mTvResponse.setText("GPIO 3 :" + res[1]);
		}
	}

	// Get GPIO_4 value
	private void GetGPIO_4_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_4_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(4);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_4_Val: failed");
			mTvResponse.setText("Get GPIO_4_Val: failed");
		} else {
			Log.d(TAG, "GPIO 4 :" + res[1]);
			mTvResponse.setText("GPIO 4 :" + res[1]);
		}
	}

	// Get GPIO_5 value
	private void GetGPIO_5_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_5_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(5);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_5_Val: failed");
			mTvResponse.setText("Get GPIO_5_Val: failed");
		} else {
			Log.d(TAG, "GPIO 5 :" + res[1]);
			mTvResponse.setText("GPIO 5 :" + res[1]);
		}
	}

	// Get GPIO_5 value
	private void GetGPIO_6_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_6_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(6);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_6_Val: failed");
			mTvResponse.setText("Get GPIO_6_Val: failed");
		} else {
			Log.d(TAG, "GPIO 6 :" + res[1]);
			mTvResponse.setText("GPIO 6 :" + res[1]);
		}
	}

	// Get GPIO_5 value
	private void GetGPIO_7_Val() {
		int[] res = new int[2];

		Log.d(TAG, "Enter  GetGPIO_7_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		res = mSerial.PL2303G_Get_GPIO_Value_B(7);

		if (res[0] < 0) {
			Log.d(TAG, "Get GPIO_7_Val: failed");
			mTvResponse.setText("Get GPIO_7_Val: failed");
		} else {
			Log.d(TAG, "GPIO 7 :" + res[1]);
			mTvResponse.setText("GPIO 7 :" + res[1]);
		}
	}

	// Set GPIO_0 value
	private void SetGPIO_0_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_0_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_0_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(0, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_0_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_0_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_0_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_0_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_1 value
	private void SetGPIO_1_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_1_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_1_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(1, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_1_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_1_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_1_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_1_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_2 value
	private void SetGPIO_2_Val() {
		int res = 0;
		int setValue = 0; // range: 0 ~ 1

		Log.d(TAG, "Enter  SetGPIO_2_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_2_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}



		if(setValue>=2) return;

		res = mSerial.PL2303G_Set_GPIO_Value_B(2, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_2_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_2_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_2_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_2_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_3 value
	private void SetGPIO_3_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_3_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_3_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(3, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_3_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_3_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_3_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_3_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_4 value
	private void SetGPIO_4_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_4_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_4_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(4, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_4_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_4_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_4_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_4_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_5 value
	private void SetGPIO_5_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_5_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_5_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(5, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_5_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_5_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_5_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_5_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_6 value
	private void SetGPIO_6_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_6_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_6_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(6, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_6_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_6_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_6_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_6_-->" + setValue + ": OK ");
		}
	}

	// Set GPIO_7 value
	private void SetGPIO_7_Val() {
		int res = 0;
		int setValue = 0;

		Log.d(TAG, "Enter  SetGPIO_7_Val");

		if (null == mSerial)
			return;

		if (!mSerial.isConnected())
			return;

		try {
			setValue = Integer.valueOf(metGPIO_7_val.getText().toString());
		}
		catch (NumberFormatException e) {

			Log.d(TAG, "Please input 0 or 1 ");
			mTvResponse.setText("Please input 0 or 1 ");
			return; 
		}

		res = mSerial.PL2303G_Set_GPIO_Value_B(7, setValue);

		if (res < 0) {
			Log.d(TAG, "SetGPIO_7_-->" + setValue + ": failed");
			mTvResponse.setText("SetGPIO_7_-->" + setValue + ": failed");
		} else {
			Log.d(TAG, "SetGPIO_7_-->" + setValue + ": OK ");
			mTvResponse.setText("SetGPIO_7_-->" + setValue + ": OK ");
		}
	}
}
