//Define Pins
#define BLUE 3
#define GREEN 5
#define RED 6
#define delayTime 10
void setup() {
  // put your setup code here, to run once:
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  digitalWrite(RED,HIGH);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);
}

int red;
int green;
int blue;
int phase = 0;
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
    analogWrite(RED, red);
    analogWrite(GREEN,green);
    analogWrite(BLUE,blue);
    delay(delayTime);
  }
  
}
