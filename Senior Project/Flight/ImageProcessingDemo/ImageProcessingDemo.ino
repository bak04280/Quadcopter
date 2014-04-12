#include <Servo.h>
#include <Wire.h>

Servo _fr;
Servo _fl;
Servo _rr;
Servo _rl;

int _baseSpeed = 1200;
int _frs, _fls, _rrs, _rls, px, py, pd;
int piVals;

void setup(){
  Serial.begin(38400);
  rotorInit();
}

void loop(){
 veryInterrupt();
}

void rotorInit(){
  _fr.attach(41);  //Attach the four rotors
  _fl.attach(39);
  _rr.attach(43);
  _rl.attach(37);                   
  signalRotors( 1000, 1000, 1000, 1000);
  delay(4000);
}

void veryInterrupt(){
 computeSignals();
 signalRotors( _frs, _fls, _rrs, _rls);
}

void computeSignals(){
  _frs = _baseSpeed - px + py - pd;
  _fls = _baseSpeed + px + py - pd;
  _rrs = _baseSpeed + px + py + pd;
  _rls = _baseSpeed - px + py + pd;
}

void signalRotors(int fr, int fl, int rr, int rl){
  _fr.writeMicroseconds(fr);
  _fl.writeMicroseconds(fl);
  _rr.writeMicroseconds(rr);
  _rl.writeMicroseconds(rl);
}

//TODO - map recieved values to rotor signal ranges, rework serialRead
void piCom(){
  if(Serial.available()){
    piVals = Serial.read();
  }
  //px = piVals[0];
  //py = piVals[1];
  //pd = piVals[2];
}

