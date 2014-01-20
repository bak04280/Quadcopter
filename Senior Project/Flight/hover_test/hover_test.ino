#include <Servo.h>
#include "TimerThree.h"

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;

int count = 0;
int _speed = 1000;

void setup(){
  Serial.begin(9600);
  _fr.attach(43);
  _rl.attach(40);
  _rr.attach(42);
  _fl.attach(41);
  _fr.writeMicroseconds(1000);
  _fl.writeMicroseconds(1000);
  _rr.writeMicroseconds(1000);
  _rl.writeMicroseconds(1000);
  Timer3.initialize(100000);
  Timer3.attachInterrupt(test);
  delay(4000);
}

void loop(){ 
  signalRotors(_speed,_speed,_speed,_speed);
}

void test(){
  count++;
  if(count > 20){
    _speed = _speed+50;
  }
  if(count >= 40){
    _speed = _speed - 100;
  }
  if(_speed <=1000 && count > 40){
    count = 0;
  }
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}


