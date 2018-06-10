int buzzers[] = {8,10,11,12,13};
int enabled[] = {0,0,0,0,0};
int state[] = {0,0,0,0,0};
int count[] = {0,0,0,0,0};
int maxCount[] = {0,0,0,0,0};
int tickInterval = 64;
unsigned int tcnt2;

void setup() {
  // put your setup code here, to run once:
  for (int i=0;i<sizeof(buzzers)/sizeof(int);i++){
    pinMode(buzzers[i], OUTPUT);
  }
  TIMSK2 &= ~(1<<TOIE2);
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
  TCCR2B &= ~(1<<WGM22);
  ASSR &= ~(1<<AS2);
  TIMSK2 &= ~(1<<OCIE2A);
  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
  TCCR2B &= ~(1<<CS21);             // Clear bit
  tcnt2 = 256 - (tickInterval/8);
  TCNT2 = tcnt2;
  TIMSK2 |= (1<<TOIE2);
  enabled[0] = 1;
  state[0] = 1;
  maxCount[0] = 13;
  digitalWrite(buzzers[0], HIGH);

  enabled[1] = 1;
  state[1] = 1;
  maxCount[1] = 9;
  digitalWrite(buzzers[1], HIGH);
}

ISR(TIMER2_OVF_vect){
  TCNT2 = tcnt2;
  for (int i=0;i<sizeof(buzzers)/sizeof(int);i++){
    if (enabled[i] == 1){
      count[i]++;
      if (count[i] >= maxCount[i]){
        if (state[i] == 0){
          digitalWrite(buzzers[i], HIGH);
          state[i] = 1;
        } else{
          digitalWrite(buzzers[i], LOW);
          state[i] = 0;
        }
        count[i] = 0;
      }
    } else{
      digitalWrite(buzzers[i], LOW);
      state[i] = 0;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
