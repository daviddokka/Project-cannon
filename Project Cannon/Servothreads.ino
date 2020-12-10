#include <Servo.h>
Servo servo1;
Servo servo2;
int currentPos = 0;
void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(8);


}
void servoWrite(int pos) {
  servo1.write(pos);
  servo2.write(pos);
  delay(50);
}
void serialReset(){
  Serial.end();
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() < 1) {};
  Serial.print("inside");
  int temp = Serial.parseInt();
  while (temp > 180 || temp < -180 || temp == 0){
    temp = Serial.parseInt();
  }
  if (currentPos < temp) {
    for (int i = currentPos; i < temp + 1; i+=5) {
      servoWrite(i);
      Serial.println(i);
    }
    servoWrite(temp);
  }
  if (currentPos > temp) {
    for (int i = currentPos; i > temp - 1; i-=5) {
      servoWrite(i);
      Serial.println(i);
    }
    servoWrite(temp);
  }
  serialReset();
  currentPos = temp;
  
}
