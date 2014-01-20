#include <Servo.h>
#include <Wire.h>
#define DEVICE (0x53)               //ADXL345 device address
#define TO_READ (6)                 //num of bytes we are going to read each time (two bytes for each axis)

byte buff[TO_READ];                 //6 bytes buffer for saving data read from the device
char str[512];                      //string buffer to transform data before sending it to the serial port

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;

int count = 0;
int x, y, z;
int _speed = 1000;
void setup(){
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
  pinMode(_throttle, INPUT);
  writeTo(DEVICE, 0x2D, 0);      //initialize gyroscope
  writeTo(DEVICE, 0x2D, 16);       
  writeTo(DEVICE, 0x2D, 8);      
  _fr.attach(43);                //attach rotors to respective variables
  _rl.attach(40);
  _rr.attach(42);
  _fl.attach(41);
  _fr.writeMicroseconds(1000);   //initialize rotors by sending throttle down signal
  _fl.writeMicroseconds(1000);
  _rr.writeMicroseconds(1000);
  _rl.writeMicroseconds(1000);
  
  delay(4000);                   //Allow time for rotors to initialize (min may be ~2000ms).
}

void loop(){
  getOrientation();
  
  if(((millis()/1000)%9)==0){
    sprintf(str, "%d %d %d", x, y, z);  
    Serial.println(str);
    delay(15);
  }
  
  signalRotors(_speed+x-y,_speed+x+y,_speed-y-x,_speed-x+y);
}

void getOrientation(){
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
   //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
   //thus we are converting both bytes in to one int
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];
   //Value conversion raw axis data -> uS servo signal. Probably doesn't belong in this function.
   //TODO: remap values to account for negative axis data
  x = map(x, 0, 390, 0, 1000); 
  y = map(y, 0, 390, 0, 1000);
  z = map(z, 0, 390, 0, 1000);
}

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

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}


