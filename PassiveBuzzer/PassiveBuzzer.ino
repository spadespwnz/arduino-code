
#define buzzer 2
int notes[] = {523,587,659,698,784,880,988,1047};
int dur = 500;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0;i<sizeof(notes)/sizeof(int);i++){
    tone(buzzer, notes[i], dur);
    delay(500);
  }
  delay(2000);
}
