#include <Servo.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "TimerThree.h"

TimerThree ISR;

int16_t ax[4], ay[4], az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;

int _baseSpeed = 1000;
int _throttle = A12;
int _frs, _fls, _rrs, _rls;
int range = 200;
float T = 0.005;
float kp = 0;
float ki = 0;
float kd = 0;
float xError, xErrorI, xOutput, xLast, xLastOut, xSetpoint;
float yError, yErrorI, yOutput, yLast, yLastOut, ySetpoint;
char buff[512];
char piVals[512];
uint8_t imubuff[14];

void setup(){
  Wire.begin();
  Serial.begin(38400);
  imuInit();
  rotorInit();
  xSetpoint = 0;
  ySetpoint = 0;
  //ISR.initialize();
  //ISR.attachInterrupt(veryInterrupt, 10000);
}

void loop(){
 //_baseSpeed = 1200;
 _baseSpeed = analogRead(_throttle);
 _baseSpeed = map(_baseSpeed, 0, 1024, 1000, 2000);
 veryInterrupt();
 //piCom();
 dogebug();
}

void rotorInit(){
  _fr.attach(41);                  //Attach the four rotors
  _fl.attach(39);
  _rr.attach(43);
  _rl.attach(37);                   
  signalRotors( 1000, 1000, 1000, 1000);
  delay(4000);
}

void veryInterrupt(){
 sampleAccel();
 manyPController();
 computeSignals();
 signalRotors( _frs, _fls, _rrs, _rls);
}

void dogebug(){
 float timer = millis();
 Serial.print(timer);
 Serial.print(", ");
 Serial.print(_baseSpeed);
 Serial.print(", ");
 Serial.print(gx);
 Serial.print(", ");
 Serial.println(ay[0]);
}

void sampleAccel(){
  I2Cdev::readBytes(0x68, 0x3B, 2, imubuff);
  ax[0] = (((int16_t)imubuff[0]) << 8) | imubuff[1];
  ax[1] = ax[0];
  ax[2] = ax[1];
  ax[3] = ax[2];
  I2Cdev::readBytes(0x68, 0x3D, 2, imubuff);
  ay[0] = (((int16_t)imubuff[0]) << 8) | imubuff[1];
  ay[1] = ay[0];
  ay[2] = ay[1];
  ay[3] = ay[2];
}

void manyPController(){
 xError = xSetpoint - (int)ax;
 xOutput = ((kp+(ki*T))*xError) + ((kp*xLast) + xLastOut);
 if(xOutput > range){
   xOutput = range;
 }
 if(xOutput < -range){
   xOutput = -range;
 }
 xLast = xError;
 xLastOut = xOutput;
 
 yError = ySetpoint - (int)ay;
 yOutput = ((kp+(ki*T))*yError) + ((kp*yLast) + yLastOut);
 if(yOutput > range){
   yOutput = range;
 }
 if(yOutput < -range){
   yOutput = -range;
 }
 yLast = yError;
 yLastOut = yOutput;
}

void computeSignals(){
  _frs = _baseSpeed +(( xOutput + yOutput));
  _fls = _baseSpeed +((-xOutput + yOutput));
  _rrs = _baseSpeed +(( xOutput - yOutput));
  _rls = _baseSpeed +((-xOutput - yOutput));
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}


void piCom(){
  if(Serial.available()){
//piVals = Serial.read();
    sprintf(buff, "%d %d", ax, ay); 
    Serial.println(buff);
  }
}

void imuInit() {
    I2Cdev::writeBits(0x68, 0x6B, 2, 3, 0x01); //Use Gyro clock
    I2Cdev::writeBits(0x68, 0x1B, 4, 2, 0x00); //Configure Gyro
    I2Cdev::writeBits(0x68, 0x1C, 4, 3, 0x00); //Configure Accel
    I2Cdev::writeBit(0x68, 0x6B, 6, false);  //Disable sleep mode
    I2Cdev::readBits(0x68, 0x75, 6, 6, imubuff);
    Serial.println((imubuff[0] ==  0x34) ? "IMU initialized" : "IMU connection failed!");    
}

//void wowsuchpid(){
//  float span  = 6.66;
//  int   error = ax - setpoint;
//  float p     = error - lastError;//ax - lastAx? doesnt matter if setpoint = 0;
//  float i     = tuneI * scanrate * error;
//  float d     = tuneD/scanrate * (error - 2*lastError + lastLastError);
//  float output = span * (p + i + d); 
//}
