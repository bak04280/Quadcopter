#include <Servo.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 imu;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;

int _count = 0;
int _baseSpeed = 0;
int _throttle = A12;
int _frs, _fls, _rrs, _rls;
int x = 0;
int y = 0;
int z = 0;
char buff[64];

void setup(){
  Wire.begin();
  Serial.begin(38400);
  imu.initialize();
   
  _fr.attach(35);                  //Attach the four rotors
  _rl.attach(32);                   
  _rr.attach(34);                   
  _fl.attach(33);                   
  _fr.writeMicroseconds(1000);     //Send stop signal to initialize rotorsInitialize motor speed as "stop"
  _fl.writeMicroseconds(1000);     
  _rr.writeMicroseconds(1000);     
  _rl.writeMicroseconds(1000);   
}

void loop(){
  _baseSpeed = analogRead(_throttle);
  _baseSpeed = map(_baseSpeed, 0, 1024, 1000, 2000);
  
  getOrientation();
}

//Read and format raw data from IMU
void getOrientation(){
  imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  ax = map(ax, -17000, 17000, -90, 90); //(pitch?) map to -90 to 90 degrees
  ay = map(ay, -17000, 17000, -90, 90); //(roll?) map to -90 to 90 degrees
  az = map(az, -17000, 17000, -90, 90); //vertical position?
  gx = map(gx, -32768, 32767, -90, 90); //x-axis acceleration mapped to arbitrary value
  gy = map(gy, -32768, 32767, -90, 90); //y-axis acceleration mapped to arbitrary value
  gz = map(gz, -32768, 32767, -90, 90); //z-axis acceleration mapped to arbitrary value
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}
