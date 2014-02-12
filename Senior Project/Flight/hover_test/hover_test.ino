#include <Servo.h>
#include <Wire.h>

#include "TimerThree.h"
#define DEVICE (0x53)               //ADXL345 device address
#define TO_READ (6)                 //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ];                 //6 bytes buffer for saving data read from the device
char str[512];                      //string buffer to transform data before sending it to the serial port

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;


int _count = 0;
int _baseSpeed = 1000;
int x,y,z;
int _throttle = A12;
int _frs, _fls, _rrs, _rls;
int xoff = 0;
int yoff = 0;
int comp[90];

void setup(){
  Wire.begin();
  Serial.begin(9600);
  for (int i = 0; i < 90; i++){
    comp[i] = 100*(1-exp((-1.7*i)/90));
    sprintf(str, "%d %d", i, comp[i]);  
    Serial.println(str);
  }
  //writeTo(DEVICE, 0x2D, 0);       //initialize ADXL345 Gyrocope
  //writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
  //writeTo(DEVICE, 0x2E, 80);     //Enable data_ready interrupt(used to set up calibration)    
  _fr.attach(35);
  _rl.attach(32);
  _rr.attach(34);
  _fl.attach(33);
  _fr.writeMicroseconds(1000);
  _fl.writeMicroseconds(1000);
  _rr.writeMicroseconds(1000);
  _rl.writeMicroseconds(1000);
  //Timer3.initialize(100000);
  //Timer3.attachInterrupt(test);
  delay(100);
  calibrate();
}



void loop(){
  _baseSpeed = analogRead(_throttle);
  _baseSpeed = map(_baseSpeed, 0, 1024, 1000, 2000);
  getOrientation();
  x = map(x, 0, 360, 0, 90); 
  y = map(y, 0, 360, 0, 90);
  sprintf(str, "%d %d", x, y);  
  Serial.println(str);
  //z = map(z, 0, 351, 0, 90);
  //x = 50*(1-exp((-1.7*x)/351));
  //y = 50*(1-exp((-1.7*y)/351));
  _frs = _baseSpeed+comp[x]-comp[y],
  _fls = _baseSpeed+comp[x]+comp[y],
  _rrs = _baseSpeed-comp[y]-comp[x],
  _rls = _baseSpeed-comp[x]+comp[y];
  signalRotors(_frs, _fls, _rrs, _rls);
  //signalRotors(_baseSpeed,_baseSpeed,_baseSpeed,_baseSpeed);

}


void signalRotors(){
  _fr.writeMicroseconds(_frs);
  _fl.writeMicroseconds(_fls);
  _rr.writeMicroseconds(_rrs);
  _rl.writeMicroseconds(_rls);
}

void signalRotors(int x){
  _fr.writeMicroseconds(x);
  _fl.writeMicroseconds(x);
  _rr.writeMicroseconds(x);
  _rl.writeMicroseconds(x);
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}

//==================Functions for Interfacing with the ADXL Gyroscope====================//
//Writes val to address register on device
void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}

//Gets x,y,z, axis data range (-351 - 0 - 351) (-90* - level - 90*)
void getOrientation(){
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
   //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
   //thus we are converting both bytes in to one int
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];
}

void calibrate(){
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  delay(15);
  for(int i=0; i<100; i++){     
    readFrom(DEVICE, regAddress, TO_READ, buff);
    xoff += (((int)buff[1]) << 8) | buff[0];   
    yoff += (((int)buff[3]) << 8) | buff[2];
  }
  xoff = -xoff/400;
  yoff = -yoff/400;
  writeTo(DEVICE, 0x1E, xoff);
  writeTo(DEVICE, 0x1f, yoff);
}



