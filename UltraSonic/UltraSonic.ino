#include <hcsr04.h>
#define buttonA 8
#define trig_pin 2
#define echo_pin 4

HCSR04 echo_mon(trig_pin, echo_pin, 20, 4000);
void setup() {
  // put your setup code here, to run once:
  pinMode (buttonA, INPUT_PULLUP);
  Serial.begin(9600);
}
bool checkDist = false;
bool buttonDown = false;
void loop() {
  // put your main code here, to run repeatedly:
  if (!buttonDown){
    if (digitalRead(buttonA) == LOW){
      buttonDown = true;
      checkDist = true;
    }
  }
  else{
    if (digitalRead(buttonA) == HIGH){
      buttonDown = false;
    }
  }
  if (checkDist){
    Serial.print(echo_mon.distanceInMillimeters());
    Serial.println("mm");
    delay(25);
    checkDist = false;
  }


}
