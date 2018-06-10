//Define Pins
#define BLUE 3
#define GREEN 5
#define RED 6
#define buttonA 8
#define buttonB 9
#define buzzer 2
#define delayTime 10
void setup() {
  // put your setup code here, to run once:
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode (buttonA, INPUT_PULLUP);
  pinMode (buttonB, INPUT_PULLUP);
  digitalWrite(RED,HIGH);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);
 
}
bool on = false;
int red;
int green;
int blue;
int phase = 0;
bool buzzerActive = false;
unsigned long buzzerStartTime = 0;
unsigned long maxBuzzerActiveTime = 1000;
void loop() {
  // put your main code here, to run repeatedly:
  red = 0;
  green = 0;
  blue = 0;
  phase++;
  if (phase > 2){
    phase = 0;
  }
  if(phase == 0){
    red=255;
  }
  if (phase == 1){
    green = 255;
  }
  if (phase == 2){
    blue = 255;
  }
  for (int i=0;i<255;i++){
    checkButton();
    if (buzzerActive){
      playBuzzer();
    }
    if (phase == 0){
      red--;
      green++;
    }
    if (phase == 1){
      green--;
      blue++;
    }
     if (phase == 2){
      blue--;
      red++;
    }
    if (on == true){
      analogWrite(RED, red);
      analogWrite(GREEN,green);
      analogWrite(BLUE,blue);
      delay(delayTime);
    } else{
      analogWrite(RED, 0);
      analogWrite(GREEN,0);
      analogWrite(BLUE,0);
      delay(delayTime);
    }
  }
}
void checkButton(){
  if (on == false){
    if (digitalRead(buttonA) == LOW){
      on = true;
      startBuzzer();
    }
  }
  else{
    if (digitalRead(buttonB) == LOW){
      startBuzzer();
      on = false;
    }
  }
}
void startBuzzer(){
  //tone(buzzer,1000);
  digitalWrite(buzzer,HIGH);
  buzzerActive = true;
  buzzerStartTime = millis();
}
void playBuzzer(){
  if (millis() - buzzerStartTime > maxBuzzerActiveTime){
    buzzerActive = false;
    // noTone(buzzer);
     digitalWrite(buzzer,LOW);
  }

  
}

