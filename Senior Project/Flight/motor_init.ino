#include <Servo.h>

Servo fr;
Servo fl;
Servo rr;
Servo rl;

int _throttle = A15;
int _speed;
void setup(){
  Serial.begin(9600);
  pinMode(_throttle, INPUT);
  fr.attach(40);
  rl.attach(41);
  rr.attach(42);
  fl.attach(43);
  fr.writeMicroseconds(1000);
  fl.writeMicroseconds(1000);
  rr.writeMicroseconds(1000);
  rl.writeMicroseconds(1000);
  delay(10000);
}

void loop(){
  _speed = analogRead(_throttle);
  _speed = map(_speed, 0, 1023, 1000, 2000);
  Serial.println(_speed);
  fr.writeMicroseconds(_speed);
  fl.writeMicroseconds(_speed);
  rr.writeMicroseconds(_speed);
  rl.writeMicroseconds(_speed);
}


