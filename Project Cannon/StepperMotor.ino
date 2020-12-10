#include <Stepper.h>
Stepper stepper;
const int stepsPerRevolution = 30;
int stepCount = 0;
const int maxStepCount = 4; //subject to change
int setStepCount = 0;
Bool reset = false;
void setup() {
  stepper.setSpeed(5); //subject to change
  Serial.begin(9600);
}
void loop() {
  if (reset = True) {
    //Reset mechanism
    reset = false;
  }
  Serial.println("Number of rounds:");
  while (Serial.available == 0);
  Serial.paserint();
  While(True) {
    stepperdrive();
    stepCount++;
    if (stepCount == maxStepCount || stepCount == setStepCount){
      reset = true;
      break;
    }
  }
}
void stepperDrive() {
  myStepper.step(stepsPerRevolution);
  delay(500);
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
