#include <RobotIRremote.h>
#include <RobotIRremoteInt.h>
#include <RobotIRremoteTools.h>

//www.elegoo.com
//2016.12.9 

#define CLK 2
#define DT 3
#define SW 4
#define IRpin 6
int tDelay = 500;
int tDelayStart = 500;
int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
int clockPin = 10;      // (9) SH_CP [SCK] on 74HC595
int dataPin = 12;     // (12) DS [S1] on 74HC595
byte leds = 0;
volatile boolean TurnDetected;
volatile boolean rotDir;
int rotPos = 0;
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  pinMode(IRpin, INPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT);
  
  digitalWrite(SW, HIGH);
  attachInterrupt(0, turn, FALLING);
}
void turn(){
  delay(4);
 
  if (digitalRead(CLK)){
    rotDir = digitalRead(DT);
  } else{
    rotDir =! digitalRead(DT);
  }

   if (rotDir){
    rotPos+=1;
  } else{
    rotPos-=1;
  }
  
   if (rotPos < 0){
    rotPos = 0;
   }
   tDelay = tDelayStart-(10*rotPos);
   if (tDelay < 0){
    tDelay = 0;
   }
   if (tDelay > tDelayStart){
    tDelay = tDelayStart;
   }

}
bool inc = true;
int ledPos = 0;
double lastUpdate = millis();
byte startCode = digitalRead(IRpin);
byte code;
void loop() 
{
  code = digitalRead(IRpin);
  if (code != startCode){
    Serial.println("IR: "+digitalRead(IRpin));
  }
  if (millis() - lastUpdate > tDelay){
    if (inc == true){
      if (ledPos >= 7){
        inc = false;
        ledPos--;
      } else{
        ledPos++;
      }
    }
    else{
      if (ledPos <= 0){
        inc = true;
        ledPos++;
      } else{
        ledPos--;
      }
    }
    lastUpdate = millis();
  }
   
  leds = 0;
  bitSet(leds, ledPos);
  updateShiftRegister();
  delay(0);
  
}

