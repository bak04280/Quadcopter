package com.example.arduinocomm;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.os.Bundle;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		_connectionEstablished = false;
		_profile = false;
		
		bTConnect();
        
	}
	
	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		bTCancel();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu); 
		return true;
	}
	
	
	public void bTConnect()
	{
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if(mBluetoothAdapter == null)
		{
			//Device does not support bluetooth
		}
		
		if(!mBluetoothAdapter.isEnabled())
		{
		   Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		   startActivityForResult(enableBtIntent, 0);
		}
		
		Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0)
        {
            for(BluetoothDevice device : pairedDevices)
            {
                if(device.getName().equals("RN42-49B8"))
                {
                    _myDevice = device;
                    break;
                }
                
            }
        }
        
        BluetoothSocket bTConnection = null;
        
        UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
        
        try{
        	bTConnection = _myDevice.createRfcommSocketToServiceRecord(myUUID);
        }
        catch(IOException e){
        	e.printStackTrace();
        }
        
        _connectionSocket = bTConnection;
        
        try{
        	_connectionSocket.connect();
        }
        catch(IOException e){
        	try{
        		_connectionSocket.close();
        	}
        	catch(IOException f){
        		return;
        	} 
        }
        
        try{
        	_myOutput = _connectionSocket.getOutputStream();
        }
        catch(IOException e){
        	e.printStackTrace();
        }
        
        
        _connectionEstablished = true;
	}
	
	public void bTCancel()
	{
		try{
			_connectionSocket.close();
			_connectionEstablished=false;
		}
		catch(IOException e){
			e.printStackTrace();
		}
	}
	
	public void powerOn(View view)
	{
		if(_connectionEstablished==false)
			bTConnect();
		if(_connectionEstablished==true)
		{
			Toast.makeText(this, "Computer Powered On", Toast.LENGTH_LONG).show();
			try {
				_myOutput.write('A');
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void increase(View view)
	{
		if(_connectionEstablished==false)
			bTConnect();
		if(_connectionEstablished==true)
		{
			Toast.makeText(this, "Increased RPM", Toast.LENGTH_LONG).show();
			try{
				_myOutput.write('B');
			}
			catch(IOException e) {
				e.printStackTrace();
			}
		}
		
		
	}
	
	public void decrease(View view)
	{
		if(_connectionEstablished==false)
			bTConnect();
		if(_connectionEstablished==true)
		{
			Toast.makeText(this, "Decreased RPM", Toast.LENGTH_LONG).show();
			try {
				_myOutput.write('C');
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public void changeProf(View view)
	{
		if(_connectionEstablished==false)
			bTConnect();
		if(_connectionEstablished==true)
		{
			if(_profile==false)
				Toast.makeText(this, "Linear Fanspeed Curve", Toast.LENGTH_LONG).show();
			if(_profile==true)
				Toast.makeText(this, "Exponential Fanspeed Curve", Toast.LENGTH_LONG).show();
			try{
				_myOutput.write('D');
			}
			catch(IOException e) {
				e.printStackTrace();
			}
		}
	}

	private boolean _connectionEstablished;
	private BluetoothDevice _myDevice;
	private BluetoothSocket _connectionSocket;
	private OutputStream _myOutput;
	private boolean _profile;
}
