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

int _baseSpeed = 0;
int _throttle = A12;
int _frs, _fls, _rrs, _rls;
int x = 0;
int y = 0;
int z = 0;
float T = 0.025;
float timer;
float kp = 1;
float ki = 1;
float kd = 0;
float xError;
float xErrorI;
float yError;
float yErrorI;
float outputX, outputY;
char buff[512];

void setup(){
  Wire.begin();
  Serial.begin(38400);
  imu.initialize();
  Serial.println("Testing device connections...");
  Serial.println(imu.testConnection() ? "imu connection successful" : "imu connection failed");
  
  _fr.attach(35);                  //Attach the four rotors
  _rl.attach(32);                   
  _rr.attach(34);                   
  _fl.attach(33);                   
  _fr.writeMicroseconds(1000);     //Send stop signal to initialize rotorsInitialize motor speed as "stop"
  _fl.writeMicroseconds(1000);     
  _rr.writeMicroseconds(1000);     
  _rl.writeMicroseconds(1000);
  delay(4000);                     //Allow time to initiate
}

void loop(){
  timer = millis();
 _baseSpeed = analogRead(_throttle);
 _baseSpeed = map(_baseSpeed, 0, 1024, 1000, 2000);
 getOrientation();
 wowsuchpid();
 signalRotors(_frs, _fls, _rrs, _rls);
 T = (0.3*((millis() - timer))+(0.6*(T)));
 T = T/1000;
 Serial.print(T);
 Serial.print(", ");
 Serial.print(outputX);
 Serial.print(", ");
 Serial.println(outputY);
}

void getOrientation(){
  imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  ax = map(ax, -17000, 17000, -90, 90);
  ay = map(ay, -17000, 17000, -90, 90);
} 

void wowsuchpid(){
 xError = ax;
 xErrorI += xError*T;
 outputX = ((xError*kp) + (xErrorI*ki));
 if(outputX > 100){
   outputX = 100;
 }
 if(outputX < -100){
   outputX = -100;
 }
 yError = ay;
 yErrorI += yError*T;
 outputY =  ((yError*kp) + (yErrorI*ki));
 if(outputY > 100){
   outputY = 100;
 }
 if(outputY < -100){
   outputY = -100;
 }
}

void computeSignals(){
  _frs = _baseSpeed - outputX - outputY;
  _fls = _baseSpeed + outputX - outputY;
  _rrs = _baseSpeed - outputX + outputY;
  _rls = _baseSpeed + outputX + outputY;
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}
