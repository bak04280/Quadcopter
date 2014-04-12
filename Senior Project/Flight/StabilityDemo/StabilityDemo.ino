#include <Servo.h>
#include <Wire.h>
#include "I2Cdev.h"

int16_t ax[4], ay[4], az;

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;

int _baseSpeed = 1200;
int _frs, _fls, _rrs, _rls;
char buff[512];
uint8_t imubuff[14];

void setup(){
  Wire.begin();
  Serial.begin(38400);
  imuInit();
  rotorInit();
}

void loop(){
 veryInterrupt();
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
 computeSignals();
 signalRotors( _frs, _fls, _rrs, _rls);
}

void sampleAccel(){
  I2Cdev::readBytes(0x68, 0x3B, 2, imubuff);
  ax[0] = (((int16_t)imubuff[0]) << 8) | imubuff[1];
  I2Cdev::readBytes(0x68, 0x3D, 2, imubuff);
  ay[0] = (((int16_t)imubuff[0]) << 8) | imubuff[1];
}

void computeSignals(){
  ax[0] = map(ax[0], -18000, 18000, -100, 100);
  ay[0] = map(ay[0], -18000, 18000, -100, 100);
  _frs = _baseSpeed -(( ax[0] + ay[0]));
  _fls = _baseSpeed -((-ax[0] + ay[0]));
  _rrs = _baseSpeed -(( ax[0] - ay[0]));
  _rls = _baseSpeed -((-ax[0] - ay[0]));
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}

void imuInit() {
    I2Cdev::writeBits(0x68, 0x6B, 2, 3, 0x01); //Use Gyro clock
    I2Cdev::writeBits(0x68, 0x1B, 4, 2, 0x00); //Configure Gyro
    I2Cdev::writeBits(0x68, 0x1C, 4, 3, 0x00); //Configure Accel
    I2Cdev::writeBit(0x68, 0x6B, 6, false);  //Disable sleep mode
    I2Cdev::readBits(0x68, 0x75, 6, 6, imubuff);  
}

