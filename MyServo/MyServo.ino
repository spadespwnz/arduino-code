#include <Servo.h>
#define servoPin 2
#define buttonA 8
#define buttonB 9
int pos = 0;
Servo servo;
void setup() {
  // put your setup code here, to run once:
  pinMode (buttonA, INPUT_PULLUP);
  pinMode (buttonB, INPUT_PULLUP);
  servo.attach(servoPin);
}

void loop() {
  if (digitalRead(buttonA) == LOW){
    if (pos < 180){
      pos++;
    }
  }
  if (digitalRead(buttonB) == LOW){
    if (pos > 0){
      pos--;
    }
  }
  servo.write(pos); 
  delay(10);

}
