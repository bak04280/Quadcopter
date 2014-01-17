#include <Servo.h>

Servo fr;
Servo fl;
Servo rr;
Servo rl;

void setup(){
  
  fr.attach(40);
  fl.attach(41);
  rr.attach(42);
  rl.attach(43);
  fr.writeMicroseconds(1000);
  fl.writeMicroseconds(1000);
  rr.writeMicroseconds(1000);
  rl.writeMicroseconds(1000);
  delay(3000);
}

void loop(){
  fr.writeMicroseconds(2000);
  fl.writeMicroseconds(2000);
  rr.writeMicroseconds(2000);
  rl.writeMicroseconds(2000);
}


