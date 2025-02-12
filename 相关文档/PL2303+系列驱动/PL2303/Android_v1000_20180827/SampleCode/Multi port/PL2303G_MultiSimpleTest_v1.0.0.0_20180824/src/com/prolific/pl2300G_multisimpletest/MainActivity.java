package com.prolific.pl2300G_multisimpletest;

import java.io.IOException;

import tw.com.prolific.pl2303gmultilib.PL2303GMultiLib;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Color;
import android.graphics.Typeface;
import android.hardware.usb.UsbManager;
import android.text.Html;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;


class UARTSettingInfo {
	public int iPortIndex = 0;
	public PL2303GMultiLib.BaudRate mBaudrate = PL2303GMultiLib.BaudRate.B115200;
	public PL2303GMultiLib.DataBits mDataBits = PL2303GMultiLib.DataBits.D8;
	public PL2303GMultiLib.Parity mParity = PL2303GMultiLib.Parity.NONE;
	public PL2303GMultiLib.StopBits mStopBits = PL2303GMultiLib.StopBits.S1;
	public PL2303GMultiLib.FlowControl mFlowControl = PL2303GMultiLib.FlowControl.OFF;		
}//class UARTSettingInfo

public class MainActivity extends Activity {

	private static boolean bDebugMesg = true;
	
	PL2303GMultiLib mSerialMulti;
	private static final int MENU_ABOUT = 0;
	
    private static enum DeviceOrderIndex {
    	DevOrder1, 
    	DevOrder2,
    	DevOrder3,
    	DevOrder4
    };
    
    
    private static final int ReadDataBufferSize = 64;
   // private static final int ReadDataBufferSize = 4096;
    
    private static final int DeviceIndex1 = 0;
    private static final int DeviceIndex2 = 1;
    private static final int DeviceIndex3 = 2;
    private static final int DeviceIndex4 = 3;
	
	private Button btOpen1;
    private Button btWrite1;
    private EditText etWrite1;
    private ScrollView svReadView1;
    private TextView tvRead1;    
   // private TextView tvSN;
    
    private Button btOpen2;
    private Button btWrite2;
    private EditText etWrite2;
    private ScrollView svReadView2;
    private TextView tvRead2;    

	private Button btOpen3;
    private Button btWrite3;
    private EditText etWrite3;
    private ScrollView svReadView3;
    private TextView tvRead3;    
    
	private Button btOpen4;
    private Button btWrite4;
    private EditText etWrite4;
    private ScrollView svReadView4;
    private TextView tvRead4;    
    
    private Spinner spBaudRate1;
    private Spinner spBaudRate2;
    private Spinner spBaudRate3;
    private Spinner spBaudRate4;
            
    private static final int MAX_DEVICE_COUNT = 4;
    private static final String ACTION_USB_PERMISSION = "com.prolific.pl2300G_multisimpletest.USB_PERMISSION";

	private static final String NULL = null;
    private UARTSettingInfo gUARTInfoList[];   
    private int iDeviceCount = 0;
    private boolean bDeviceOpened[] = new boolean[MAX_DEVICE_COUNT];
    
    private boolean gThreadStop[] = new boolean[MAX_DEVICE_COUNT];
    private boolean gRunningReadThread[] = new boolean[MAX_DEVICE_COUNT];
    private boolean enableFixedCOMPortMode=false;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		DumpMsg("Enter onCreate");
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		spBaudRate1 = (Spinner)findViewById(R.id.DevSpinner1);
		spBaudRate2 = (Spinner)findViewById(R.id.DevSpinner2);
		spBaudRate3 = (Spinner)findViewById(R.id.DevSpinner3);
		spBaudRate4 = (Spinner)findViewById(R.id.DevSpinner4);
		ArrayAdapter<CharSequence> adapter = 
				ArrayAdapter.createFromResource(this, R.array.BaudRateList, android.R.layout.simple_spinner_item);

		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		
		spBaudRate1.setAdapter(adapter);		
		spBaudRate1.setOnItemSelectedListener(new MyOnItemSelectedListener());
		spBaudRate1.setSelection(5);//baudrate = 9600, base is 0
		spBaudRate1.setEnabled(false);

		spBaudRate2.setAdapter(adapter);		
		spBaudRate2.setOnItemSelectedListener(new MyOnItemSelectedListener());
		spBaudRate2.setSelection(5);//baudrate = 9600, base is 0
		spBaudRate2.setEnabled(false);

		spBaudRate3.setAdapter(adapter);		
		spBaudRate3.setOnItemSelectedListener(new MyOnItemSelectedListener());
		spBaudRate3.setSelection(5);//baudrate = 9600, base is 0
		spBaudRate3.setEnabled(false);		
		
		spBaudRate4.setAdapter(adapter);		
		spBaudRate4.setOnItemSelectedListener(new MyOnItemSelectedListener());
		spBaudRate4.setSelection(5);//baudrate = 9600, base is 0
		spBaudRate4.setEnabled(false);
		
		btOpen1 = (Button)findViewById(R.id.OpenButton1);
		btOpen1.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {		
				OpenUARTDevice(DeviceIndex1);
			}
		});
		btOpen1.setEnabled(false);
		
		btWrite1 = (Button)findViewById(R.id.WriteButton1);
		btWrite1.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {
					WriteToUARTDevice(DeviceIndex1);
			}
		});	
		etWrite1 = (EditText) findViewById(R.id.StrText1);		
		btWrite1.setEnabled(false);
		etWrite1.setEnabled(false);
		//tvSN = (TextView) findViewById(R.id.tvSNumber);
		
		svReadView1 = (ScrollView)findViewById(R.id.svScrollView1);
		tvRead1 = (TextView)findViewById(R.id.tvText1);		
		
		btOpen2 = (Button)findViewById(R.id.OpenButton2);
		btOpen2.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {
				OpenUARTDevice(DeviceIndex2);
			}
		});
		btOpen2.setEnabled(false);
		
		btWrite2 = (Button)findViewById(R.id.WriteButton2);
		btWrite2.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {	
					WriteToUARTDevice(DeviceIndex2);
			}
		});
		etWrite2 = (EditText) findViewById(R.id.StrText2);
		btWrite2.setEnabled(false);
		etWrite2.setEnabled(false);
		
		svReadView2 = (ScrollView)findViewById(R.id.svScrollView2);
		tvRead2 = (TextView)findViewById(R.id.tvText2);

		btOpen3 = (Button)findViewById(R.id.OpenButton3);
		btOpen3.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {		
				OpenUARTDevice(DeviceIndex3);
			}
		});
		btOpen3.setEnabled(false);
		
		btWrite3 = (Button)findViewById(R.id.WriteButton3);
		btWrite3.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {	
					WriteToUARTDevice(DeviceIndex3);
			}
		});	
		etWrite3 = (EditText) findViewById(R.id.StrText3);
		btWrite3.setEnabled(false);
		etWrite3.setEnabled(false);
		
		svReadView3 = (ScrollView)findViewById(R.id.svScrollView3);
		tvRead3 = (TextView)findViewById(R.id.tvText3);		
		
		
		btOpen4 = (Button)findViewById(R.id.OpenButton4);
		btOpen4.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {		
				OpenUARTDevice(DeviceIndex4);
			}
		});
		btOpen4.setEnabled(false);
		
		btWrite4 = (Button)findViewById(R.id.WriteButton4);
		btWrite4.setOnClickListener(new Button.OnClickListener() {		
			public void onClick(View v) {	
					WriteToUARTDevice(DeviceIndex4);
			}
		});	
		etWrite4 = (EditText) findViewById(R.id.StrText4);
		btWrite4.setEnabled(false);
		etWrite4.setEnabled(false);
		
		svReadView4 = (ScrollView)findViewById(R.id.svScrollView4);
		tvRead4 = (TextView)findViewById(R.id.tvText4);	
		
	    // get service
		mSerialMulti = new PL2303GMultiLib((UsbManager) getSystemService(Context.USB_SERVICE),
           	  	this, ACTION_USB_PERMISSION); 	
		//if you don't want to use Software Queue, below constructor to be used
		//mSerialMulti = new PL2303GMultiLib((UsbManager) getSystemService(Context.USB_SERVICE),
        //   	  	this, ACTION_USB_PERMISSION,false); 						
		
		gUARTInfoList = new UARTSettingInfo[MAX_DEVICE_COUNT];
		
		// enable Fixed com port mode;
		//enableFixedCOMPortMode=mSerialMulti.EnableFixed_COMPort_Mode();
		// enable Fixed com port mode;
		
		for(int i=0;i<MAX_DEVICE_COUNT;i++) {
			gUARTInfoList[i] = new UARTSettingInfo(); 	
			gUARTInfoList[i].iPortIndex = i;
		    gThreadStop[i] = false;
		    gRunningReadThread[i] = false;	
		    bDeviceOpened[i] = false;
		}
		
		DumpMsg("Leave onCreate");
	}

	public void onPause() {
	    	DumpMsg("Enter onPause");
	    	super.onStart();
	    	DumpMsg("Leave onPause");
	}

	public void onRestart() {
    	DumpMsg("Enter onRestart");
    	//super.onStart();
    	super.onRestart();
    	DumpMsg("Leave onRestart");
	}
	
   	protected void onStop() {
    	DumpMsg("Enter onStop");
    	super.onStop();        
    	DumpMsg("Leave onStop");
    }    

    protected void onDestroy() {
    	DumpMsg("Enter onDestroy");     
    	if(mSerialMulti!=null) {
    		for(int i=0;i<MAX_DEVICE_COUNT;i++) {
    		    gThreadStop[i] = true;
    		}//First to stop app view-thread
    		if(iDeviceCount>0)
    			unregisterReceiver(PLMultiLibReceiver);
    		mSerialMulti.PL2303Release();
    		mSerialMulti = null;
    	}
    	super.onDestroy();        
    	DumpMsg("Leave onDestroy");
    }    

    public void onStart() {
    	DumpMsg("Enter onStart");
    	super.onStart();
    	DumpMsg("Leave onStart");
    }
    
    public void onResume() {
    	DumpMsg("Enter onResume"); 
    	super.onResume();
    	String action =  getIntent().getAction();
    	DumpMsg("onResume:"+action);       	
    	
    	synchronized (this) {
    	   ReSetStatus();
    	
    	
   		iDeviceCount = mSerialMulti.PL2303Enumerate();
   		
   		DelayTime(60);
   		
       	DumpMsg("enumerate Count="+iDeviceCount);
       	if( 0==iDeviceCount ) {
       		SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder1,false,false);
       		SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder2,false,false);
       		SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder3,false,false);
       		SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder4,false,false);
       		Toast.makeText(this, "no more devices found", Toast.LENGTH_SHORT).show();    
       		DumpMsg("no more devices found");  
       	} else {   
       		DumpMsg("DevOpen[0]="+bDeviceOpened[DeviceIndex1]);
       		DumpMsg("DevOpen[1]="+bDeviceOpened[DeviceIndex2]);
       		DumpMsg("DevOpen[2]="+bDeviceOpened[DeviceIndex3]);
       		DumpMsg("DevOpen[3]="+bDeviceOpened[DeviceIndex4]);
       		
       		if(!bDeviceOpened[DeviceIndex1]) {
       			DumpMsg("iDeviceCount(=1)="+iDeviceCount);
       			SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder1, true, false);       
       	 	    if(enableFixedCOMPortMode) {
       	 	    	if(mSerialMulti.PL2303getCOMNumber(0)!=NULL || mSerialMulti.PL2303getCOMNumber(0)!="" ) {
       	 	    	btOpen1.setText(mSerialMulti.PL2303getCOMNumber(0));
       	 	        DumpMsg("Button1_COM Number: "+ mSerialMulti.PL2303getCOMNumber(0));
       	 	    	}
       	 	    }       		 	
       		}
       		if(iDeviceCount>=2) {
       			DumpMsg("iDeviceCount(>=2)="+iDeviceCount);
       			if(!bDeviceOpened[DeviceIndex2]) {
       				SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder2, true, false);
       			 if(enableFixedCOMPortMode) {
       				if(mSerialMulti.PL2303getCOMNumber(1)!=NULL || mSerialMulti.PL2303getCOMNumber(1)!="" ) {
       			     DumpMsg("Button2_COM Number: "+ mSerialMulti.PL2303getCOMNumber(1));
       			     btOpen2.setText(mSerialMulti.PL2303getCOMNumber(1));
       				}
       			 }
       			}
       			if(iDeviceCount==2) {
       				SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder3, false, false);
       			}
       		} 
       		if(iDeviceCount>=3) {
       			DumpMsg("iDeviceCount(>=3)="+iDeviceCount);
       			if(!bDeviceOpened[DeviceIndex3]) {
       				SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder3, true, false);
       			 if(enableFixedCOMPortMode) {
       				if(mSerialMulti.PL2303getCOMNumber(2)!=NULL || mSerialMulti.PL2303getCOMNumber(2)!="" ) {
       			 DumpMsg("Button3_COM Number: "+ mSerialMulti.PL2303getCOMNumber(2));
       			btOpen3.setText(mSerialMulti.PL2303getCOMNumber(2));
       			 }
       			 }
       			}
       			if(iDeviceCount==3) {
       				SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder4, false, false);
       			}
       		} 
       		
       		if(iDeviceCount>=4){
       			DumpMsg("iDeviceCount(>=4)="+iDeviceCount);
       			if(!bDeviceOpened[DeviceIndex4]) {
       				SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder4, true, false);
       			 if(enableFixedCOMPortMode) {
       				if(mSerialMulti.PL2303getCOMNumber(3)!=NULL || mSerialMulti.PL2303getCOMNumber(3)!="" ) {
       			 DumpMsg("Button4_COM Number: "+ mSerialMulti.PL2303getCOMNumber(3));
       			btOpen4.setText(mSerialMulti.PL2303getCOMNumber(3));
       			 }
       			 }
       			}
       		} 
       		//register receiver for PL2303Multi_USB notification
       		IntentFilter filter = new IntentFilter();
       	    filter.addAction(mSerialMulti.PLUART_MESSAGE); 
       	    registerReceiver(PLMultiLibReceiver, filter);
   			Toast.makeText(this, "The "+iDeviceCount+" devices are attached", Toast.LENGTH_SHORT).show();
   			DumpMsg("The "+iDeviceCount+" devices are attached"); 
       	}//if( 0==iDevCnt )       
       	
    	 }
    	DumpMsg("Leave onResume"); 
    }//public void onResume()         	

    private final BroadcastReceiver PLMultiLibReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
           if(intent.getAction().equals(mSerialMulti.PLUART_MESSAGE)){
        	   Bundle extras = intent.getExtras();
        	   if(extras!=null) {
        		   String str = (String)extras.get(mSerialMulti.PLUART_DETACHED);
        		   DumpMsg("receive data:"+str);
        		   int index = Integer.valueOf(str);
        		   if(DeviceIndex1==index) {
               		   SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder1,false,false);   
               		   spBaudRate1.setEnabled(false);
               		   bDeviceOpened[DeviceIndex1] = false;
               		 if(enableFixedCOMPortMode) btOpen1.setText("Open");
               		   
               		 //  tvSN.setTextColor(0xffff0000);
               		 //  tvSN.setText("");               		   
        		   } else if(DeviceIndex2==index) {
               		   SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder2,false,false);
               		   spBaudRate2.setEnabled(false);
               		   bDeviceOpened[DeviceIndex2] = false;
               		 if(enableFixedCOMPortMode) btOpen2.setText("Open");
        		   } else if(DeviceIndex3==index) {
        			   SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder3,false,false);
        			   spBaudRate3.setEnabled(false);
        			   bDeviceOpened[DeviceIndex3] = false;
        			   if(enableFixedCOMPortMode) btOpen3.setText("Open");
        		   } else if(DeviceIndex4==index) {
        			   SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder4,false,false);
        			   spBaudRate4.setEnabled(false);
        			   bDeviceOpened[DeviceIndex4] = false;
        			   if(enableFixedCOMPortMode) btOpen4.setText("Open");
        		   }
        		   
        	   }        	   
           }    
        }//onReceive
     };
    
    private void SetEnabledDevControlPanel(DeviceOrderIndex iDev, boolean bOpen, boolean bWrite) {
    	switch(iDev) {
    		case DevOrder1:
    	   		spBaudRate1.setEnabled(true);
    	   		btOpen1.setEnabled(bOpen); 
    	   		btWrite1.setEnabled(bWrite);	 
    	   		etWrite1.setEnabled(bWrite);
    			break;
    		case DevOrder2:
    			spBaudRate2.setEnabled(true);
    			btOpen2.setEnabled(bOpen); 
    			btWrite2.setEnabled(bWrite);
    			etWrite2.setEnabled(bWrite);
    			break;
    		case DevOrder3:
    			spBaudRate3.setEnabled(true);
    			btOpen3.setEnabled(bOpen); 
    			btWrite3.setEnabled(bWrite);
    			etWrite3.setEnabled(bWrite);
    			break;
    		case DevOrder4:
    			spBaudRate4.setEnabled(true);
    			btOpen4.setEnabled(bOpen); 
    			btWrite4.setEnabled(bWrite);
    			etWrite4.setEnabled(bWrite);
    			break;    			
    	}
    }
    
    private void OpenUARTDevice(int index) {
    	
    	DumpMsg("Enter OpenUARTDevice: "+String.valueOf(index));
    	
    	// Toast.makeText(this, "Enter OpenUARTDevice: Index"+String.valueOf(index), Toast.LENGTH_SHORT).show();
    	 
   	 	if(mSerialMulti==null) {
   	 	    //Toast.makeText(this, "Error: mSerialMulti==null", Toast.LENGTH_SHORT).show();
   	 	    DumpMsg("Error: mSerialMulti==null");
   	    	return;  
   	 	}
   	 			
   	 	
        if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(index)) {
        	  //Toast.makeText(this, "error:!AP_mSerialMulti.PL2303IsDeviceConnectedByIndex(index)", Toast.LENGTH_SHORT).show();
        	  DumpMsg("Error: !AP_mSerialMulti.PL2303IsDeviceConnectedByIndex(index)");
         	return;  	   	 	
        }
    	
		boolean res;
		UARTSettingInfo info = gUARTInfoList[index];
		
		//Toast.makeText(this, "UARTSettingInfo: index:"+String.valueOf(info.iPortIndex), Toast.LENGTH_SHORT).show();
		 DumpMsg("UARTSettingInfo: index:"+String.valueOf(info.iPortIndex));
		
		
		res = mSerialMulti.PL2303OpenDevByUARTSetting(index, info.mBaudrate, info.mDataBits, info.mStopBits, 
					info.mParity, info.mFlowControl);
		if( !res ) {
			DumpMsg("Error: fail to PL2303OpenDevByUARTSetting");
			//Toast.makeText(this, "AP_ Can't set UART correctly!", Toast.LENGTH_SHORT).show();
			return;
		}              			
        
		if(DeviceIndex1==index) {
			SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder1, false, true);		
			//tvSN.setTextColor(0xff00ff00);
			//tvSN.setText(mSerialMulti.PL2303Device_GetSerialNumber(index));			
		} else if(DeviceIndex2==index) {
			SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder2, false, true);
		} else if(DeviceIndex3==index) {
			SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder3, false, true);
		} else if(DeviceIndex4==index) {
			SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder4, false, true);
		}
		
		bDeviceOpened[index] = true;
		
		if(!gRunningReadThread[index]) {
			UpdateDisplayView(index);
		}
		
   	 	DumpMsg("Open ["+ mSerialMulti.PL2303getDevicePathByIndex(index) +"] successfully!");
   	    // Toast.makeText(this, "Leave OpenUARTDevice", Toast.LENGTH_SHORT).show();
   	 	Toast.makeText(this, "Open ["+ mSerialMulti.PL2303getDevicePathByIndex(index) +"] successfully!", Toast.LENGTH_SHORT).show();
   	 	
   	    DumpMsg("Open ["+ mSerialMulti.PL2303getCOMNumber(index) +"] successfully!");
	    // Toast.makeText(this, "Leave OpenUARTDevice", Toast.LENGTH_SHORT).show();
	 	Toast.makeText(this, "Open ["+ mSerialMulti.PL2303getCOMNumber(index) +"] successfully!", Toast.LENGTH_SHORT).show();
	 	
    	return;
    }//private void OpenUARTDevice(int index)
    
    private void UpdateDisplayView(int index) {
    	gThreadStop[index] = false;
	    gRunningReadThread[index] = true;	 
	    
    	if( DeviceIndex1==index ) {
    		new Thread(ReadLoop1).start();    		
    	} else if( DeviceIndex2==index ) {
    		new Thread(ReadLoop2).start();
    	} else if( DeviceIndex3==index ) { 
    		new Thread(ReadLoop3).start();
    	} else if( DeviceIndex4==index ) { 
    		new Thread(ReadLoop4).start();
    	}
    }        
    
    private int ReadLen1;
    private byte[] ReadBuf1 = new byte[ReadDataBufferSize];    
    Handler mHandler1 = new Handler();
    private Runnable ReadLoop1 = new Runnable() {
        public void run() {
                        
            for (;;) {
            	
            	ReadLen1 = mSerialMulti.PL2303Read(DeviceIndex1, ReadBuf1);
                if (ReadLen1 > 0) {
                    //ReadBuf1[ReadLen1] = 0;                	
                 	DumpMsg("Read  Length : " + ReadLen1);
                 	mHandler1.post(new Runnable() {                 		
                 		public void run() {
                 			StringBuffer sbHex=new StringBuffer();
                 	         for (int j = 0; j < ReadLen1; j++) {            	   
                 	        	 sbHex.append((char) (ReadBuf1[j]&0x000000FF));
                             }              
                 	         tvRead1.setText(sbHex.toString());
                 	         svReadView1.fullScroll(ScrollView.FOCUS_DOWN);
                 		}//run
                 	});//Handler.post
                }//if (len > 0)

                DelayTime(60);

                if (gThreadStop[DeviceIndex1]) {
                	gRunningReadThread[DeviceIndex1] = false;
                	return;
                }//if                
            }//for(...)
            
        }//run
    };//Runnable    
     
    private int ReadLen2;
    private byte[] ReadBuf2 = new byte[ReadDataBufferSize];    
    Handler mHandler2 = new Handler();
    private Runnable ReadLoop2 = new Runnable() {
        public void run() {
                        
            for (;;) {
            	
            	ReadLen2 = mSerialMulti.PL2303Read(DeviceIndex2, ReadBuf2);            	
                if (ReadLen2 > 0) {
                	//ReadBuf2[ReadLen2] = 0;
                 	DumpMsg("Read  Length : " + ReadLen2);
                 	mHandler2.post(new Runnable() {
                 		StringBuffer sbHex=new StringBuffer();
                 		public void run() {
                 	         for (int j = 0; j < ReadLen2; j++) {            	   
                 	        	 sbHex.append((char) (ReadBuf2[j]&0x000000FF));
                             }              
                 	         tvRead2.setText(sbHex.toString());    
                 	         svReadView2.fullScroll(ScrollView.FOCUS_DOWN);
                 		}//run
                 	});//Handler.post
                }//if (len > 0)

                DelayTime(60);
                
                if (gThreadStop[DeviceIndex2]) {
                	gRunningReadThread[DeviceIndex2] = false;
                	return;
                }//if                
            }//for(...)
        }//run
    };//Runnable       
    
    private int ReadLen3;
    private byte[] ReadBuf3 = new byte[ReadDataBufferSize];    
    Handler mHandler3 = new Handler();
    private Runnable ReadLoop3 = new Runnable() {
        public void run() {
                        
            for (;;) {
            	
            	ReadLen3 = mSerialMulti.PL2303Read(DeviceIndex3, ReadBuf3);
                if (ReadLen3 > 0) {
                    //ReadBuf3[ReadLen3] = 0;                	
                 	DumpMsg("Read  Length : " + ReadLen3);
                 	mHandler3.post(new Runnable() {
                 		StringBuffer sbHex=new StringBuffer();
                 		public void run() {
                 	         for (int j = 0; j < ReadLen3; j++) {            	   
                 	        	 sbHex.append((char) (ReadBuf3[j]&0x000000FF));
                             }         
                 	         tvRead3.setText(sbHex.toString());                    			
                 	         svReadView3.fullScroll(ScrollView.FOCUS_DOWN);
                 		}//run
                 	});//Handler.post
                }//if (len > 0)

                DelayTime(60);

                if (gThreadStop[DeviceIndex3]) {
                	gRunningReadThread[DeviceIndex3] = false;
                	return;
                }//if                
            }//for(...)
            
        }//run
    };//Runnable    
    
    private int ReadLen4;
    private byte[] ReadBuf4 = new byte[ReadDataBufferSize];    
    Handler mHandler4 = new Handler();
    private Runnable ReadLoop4 = new Runnable() {
        public void run() {
                        
            for (;;) {
            	ReadLen4 = mSerialMulti.PL2303Read(DeviceIndex4, ReadBuf4);
                if (ReadLen4 > 0) {
                    //ReadBuf3[ReadLen3] = 0;                	
                 	DumpMsg("Read  Length : " + ReadLen4);
                 	mHandler4.post(new Runnable() {
                 		StringBuffer sbHex=new StringBuffer();
                 		public void run() {
                 	         for (int j = 0; j < ReadLen4; j++) {            	   
                 	        	 sbHex.append((char) (ReadBuf4[j]&0x000000FF));
                             }         
                 	         tvRead4.setText(sbHex.toString());                    			
                 	         svReadView4.fullScroll(ScrollView.FOCUS_DOWN);
                 		}//run
                 	});//Handler.post
                }//if (len > 0)

                DelayTime(60);

                if (gThreadStop[DeviceIndex4]) {
                	gRunningReadThread[DeviceIndex4] = false;
                	return;
                }//if                
            }//for(...)
            
        }//run
    };//Runnable 
    
    private void ReSetStatus(){
    	
    	DumpMsg("-->> ReSetStatus");
    	
    	DumpMsg("-->> PL2303HXD_ReSetStatus");
    	
    	mSerialMulti.PL2303HXD_ReSetStatus();
    	
    	DumpMsg("<<-- PL2303HXD_ReSetStatus");
   	if(bDeviceOpened[DeviceIndex1]) {
    		
    	 	DumpMsg("DeviceIndex1 is open");
    	 	if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(0)) {
    	 		DumpMsg("DeviceIndex1: disconnect");

    	 		 SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder1,false,false);   
       		   spBaudRate1.setEnabled(false);
       		   bDeviceOpened[DeviceIndex1] = false;
       		 if(enableFixedCOMPortMode) btOpen1.setText("Open");
    	 	}
    		
    	}
     	if(bDeviceOpened[DeviceIndex2]) {
    		
    	 	DumpMsg("DeviceIndex2 is open");
    	 	if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(1)) {
    	 		DumpMsg("DeviceIndex2: disconnect");
   	 		 SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder2,false,false);   
     		   spBaudRate2.setEnabled(false);
     		   bDeviceOpened[DeviceIndex2] = false;
     		  if(enableFixedCOMPortMode) btOpen2.setText("Open");
    	 	}
    		
    	}
   	    if(bDeviceOpened[DeviceIndex3]) {
		
	 	DumpMsg("DeviceIndex3 is open");
	 	if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(2)) {
	 		DumpMsg("DeviceIndex3: disconnect");
	 		 SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder3,false,false);   
     		   spBaudRate3.setEnabled(false);
     		   bDeviceOpened[DeviceIndex3] = false;
     		  if(enableFixedCOMPortMode) btOpen3.setText("Open");
	 	}
		
	    }
   	    
     	if(bDeviceOpened[DeviceIndex4]) {
		
	 	DumpMsg("DeviceIndex4 is open");
	 	if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(3)) {
	 		DumpMsg("DeviceIndex4: disconnect");
	 		 SetEnabledDevControlPanel(DeviceOrderIndex.DevOrder4,false,false);   
     		   spBaudRate4.setEnabled(false);
     		   bDeviceOpened[DeviceIndex4] = false;
     		  if(enableFixedCOMPortMode) btOpen4.setText("Open");
	 	}
		
	    }
     	DumpMsg("<<-- ReSetStatus");
    	
    }
    
    private void WriteToUARTDevice(int index) {
    	DumpMsg("Enter WriteToUARTDevice");
    	
   	 	if(mSerialMulti==null)
   	 		return;  	
   	 	
        if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(index)) 
         	return;  	  
        
        String strWrite = null;
        if(DeviceIndex1==index) {
        	strWrite = etWrite1.getText().toString();
        } else if(DeviceIndex2==index) {
        	strWrite = etWrite2.getText().toString();
        } else if(DeviceIndex3==index) {
        	strWrite = etWrite3.getText().toString();
        } else if(DeviceIndex4==index) {
        	strWrite = etWrite4.getText().toString();
        }
        //DumpMsg("PL2303Multi Write(" + strWrite.length() + "):" + strWrite);
        
        if( strWrite==null || "".equals(strWrite.trim()) ) { //str is empty
        	DumpMsg("WriteToUARTDevice: no data to write");
        	return;
        }

        int res = mSerialMulti.PL2303Write(index, strWrite.getBytes());
    	if( res<0 ) {
    		DumpMsg("w: fail to write: "+ res);
    		return;
    	}         	

        DumpMsg("Leave WriteToUARTDevice");
    } //private void WriteToUARTDevice(int index)
    
    
 	public class MyOnItemSelectedListener implements OnItemSelectedListener {
  		public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {  			
  			 Spinner spinner = (Spinner) parent;
  			 String newBaudRate = spinner.getItemAtPosition(position).toString();
  			 int iBaudRate=0, iSelected = 0; 
  			 UARTSettingInfo info = new UARTSettingInfo(); 
  			 PL2303GMultiLib.BaudRate rate;
  			 
  			 DumpMsg("-->>MyOnItemSelectedListener-->>onItemSelected: "+spinner.getId());

 			 if(mSerialMulti==null)
  				 return;  			 
  			   			 
  		     if(R.id.DevSpinner1 == spinner.getId())  {
  		    	iSelected = DeviceIndex1;
  		     } else if(R.id.DevSpinner2 == spinner.getId()) {
  		    	iSelected = DeviceIndex2;
  		     } else if(R.id.DevSpinner3 == spinner.getId()) {
  		    	iSelected = DeviceIndex3; 
  		     } else if(R.id.DevSpinner4 == spinner.getId()) {
   		    	iSelected = DeviceIndex4; 
   		     }
  		     
             if(!mSerialMulti.PL2303IsDeviceConnectedByIndex(iSelected)) {
            	 
            	 DumpMsg("-->>MyOnItemSelectedListener-->>onItemSelected_PL2303IsDeviceConnectedByIndex");
            	 
             	return;  		     
             }
  		     
             info.iPortIndex = iSelected;
             
  			 try {
  				iBaudRate= Integer.parseInt(newBaudRate);
  			 }
  			 catch (NumberFormatException e)	{
  				System.out.println(" parse int error!!  " + e);
  			 }  			
  			
			 switch (iBaudRate) {
      		 	case 75:
      		 		rate = PL2303GMultiLib.BaudRate.B75;
      		 		break;        		 
      		 	case 300:
      		 		rate = PL2303GMultiLib.BaudRate.B300;
      		 		break;        		 
      		 	case 1200:
      		 		rate = PL2303GMultiLib.BaudRate.B1200;
      		 		break;        		 
      		 	case 2400:
      		 		rate = PL2303GMultiLib.BaudRate.B2400;
      		 		break;    
      		 	case 4800:
      		 		rate = PL2303GMultiLib.BaudRate.B4800;
      		 		break;  
      		 	case 9600:
      		 		rate = PL2303GMultiLib.BaudRate.B9600;
      		 		break;        	
      		 	case 14400:
      		 		rate = PL2303GMultiLib.BaudRate.B14400;
      		 		break;
      		 	case 19200:
      		 		rate = PL2303GMultiLib.BaudRate.B19200;
      		 		break;        		 
      		 	case 57600:
      		 		rate = PL2303GMultiLib.BaudRate.B57600;
      		 		break;        		 
      		 	case 115200:
      		 		rate = PL2303GMultiLib.BaudRate.B115200;      		 		
      		 		break;        		 
      		 	case 614400:
      		 		rate = PL2303GMultiLib.BaudRate.B614400;      		 		
      		 		break;     
      		 	case 921600:
      		 		rate = PL2303GMultiLib.BaudRate.B921600;
      		 		break;
      		 	case 1228800:
      		 		rate = PL2303GMultiLib.BaudRate.B1228800;
      		 		break;
      		 	case 3000000:
      		 		rate = PL2303GMultiLib.BaudRate.B3000000;
      		 		break;
      		 	case 6000000:
      		 		rate = PL2303GMultiLib.BaudRate.B6000000;
      		 		break;
      		 	default:
      		 		rate = PL2303GMultiLib.BaudRate.B9600;
      		 		break;        		  
      		 }   			   			   			 
             info.mBaudrate = rate;
             
 			int res = 0;
 			try {
 				DumpMsg("iSelected:"+iSelected+";Baudrate:"+rate);
 				res = mSerialMulti.PL2303SetupCOMPort(iSelected, info.mBaudrate, info.mDataBits, info.mStopBits, 
 						info.mParity, info.mFlowControl);
				gUARTInfoList[iSelected] = info;	
 			} catch (IOException e) {
 				e.printStackTrace();
 			}
 			if( res<0 ) {
 				DumpMsg("fail to setup");
 				return;
 			}              			
  		}//public void onItemSelected
 		public void onNothingSelected(AdapterView<?> parent) {
  			// Do nothing.    
  		}
 	}
 	
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		// getMenuInflater().inflate(R.menu.activity_main, menu);
		menu.add(0, MENU_ABOUT, 0, "About");
		return true;		
	} 	
 	
	public boolean onOptionsItemSelected(MenuItem item) {

		switch (item.getItemId()) {
			case MENU_ABOUT:
				TextView about_dialog = new TextView(this);
				byte[] byVersion = new byte[16];
				PackageInfo pinfo = null;
				StringBuffer sbHex=new StringBuffer();
				
				mSerialMulti.PL2303LibGetVersion(byVersion);				
    	         for (int j = 0; j < byVersion.length; j++) {            	   
    	        	 sbHex.append((char)byVersion[j]);
                }                             				
				
				try {
					pinfo = getPackageManager().getPackageInfo(getPackageName(), 0);
				} catch (NameNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				//int versionNumber = pinfo.versionCode;
				String versionName = pinfo.versionName;
				
				about_dialog.setTypeface(Typeface.SANS_SERIF);
				about_dialog.setText(Html.fromHtml("Welcome to PL2303 Android Demo Program with Multi-Ports Library<br><br>Library Version: " +  
						sbHex.toString() + "<br>Program Version: " + versionName +"<br>"+
						"USB Host Feature Supported: " + mSerialMulti.PL2303USBFeatureSupported() +"<br><br>"+
						"If you have any problem, please choose the helper contact us<br><br>"+
						"Best Regards,<br>Prolific Tech.<br>"));				
				about_dialog.setTextColor(Color.BLACK);
				about_dialog.setTextSize(20);
				about_dialog.setPadding(20, 0, 0, 0);
				about_dialog.setLineSpacing(3.4f, 1.2f);
				about_dialog.setMovementMethod(ScrollingMovementMethod.getInstance());
				
				new AlertDialog.Builder(this)
				.setTitle("About")
				.setView(about_dialog)
				.setPositiveButton("Close", null)
				.show();				
				break;
			
			default:
				break;
		}
		
		return true;
	}
 	  //--------------------------------------------------------------------------------------------------------//
 	  //--------------------------------------------------------------------------------------------------------//
 	  //--------------------------------------------------------------------------------------------------------//
 	  //--------------------------------------------------------------------------------------------------------//
 	  //--------------------------------------------------------------------------------------------------------//	
 	    /*
 	     * Miscellaneous functions
 	     * 
 	     */
	private void DelayTime(int dwTimeMS) {
		//Thread.yield();
		long StartTime, CheckTime;
				
		if(0==dwTimeMS) {
			Thread.yield();
			return;		
		}
		//Returns milliseconds running in the current thread
		StartTime = System.currentTimeMillis();
		do {
				CheckTime=System.currentTimeMillis();
				Thread.yield();
		 } while( (CheckTime-StartTime)<=dwTimeMS);		
	}	
	
 	private static void DumpMsg(Object s) {
 		if(true==bDebugMesg) {
 			//Log.d("PL2303MultiUSBApp", ">==< " + s.toString() + " >==<");
 			Log.d("PL2303MultiUSBApp", "A: " + s.toString());
 		}
 	} 	
}
