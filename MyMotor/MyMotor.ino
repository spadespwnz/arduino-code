#define ENABLE 3
#define DIRA 5
#define DIRB 7

void setup() {
  // put your setup code here, to run once:
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
  digitalWrite(ENABLE,HIGH);
  digitalWrite(DIRA,HIGH);
  digitalWrite(DIRB,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*Serial.println("One way, then reverse");
  digitalWrite(ENABLE,HIGH);
  digitalWrite(DIRA,HIGH);
  digitalWrite(DIRB,LOW);
  delay(1000);
  digitalWrite(ENABLE,LOW);
  delay(1000);
  */
}
