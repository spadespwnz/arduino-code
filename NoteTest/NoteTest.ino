int buzzers[] = {4,10,11,12,13};
int buzzer_states[] = {0,0,0,0,0};
int timer_counter[] = {-1,-1,-1,-1,-1};
int timer_counter_r[] = {-1,-1,-1,-1,-1};
int buzzer_states2[] = {0,0,0,0,0};
int timer_counter2[] = {-1,-1,-1,-1,-1};
int timer_counter2_r[] = {-1,-1,-1,-1,-1};
const int maxSpeakers = 5;
//const float PI = 3.1415;
const float U_HZ = 4500;
const int U_FREQ = 16000000 / (1.0*int(U_HZ)) ;
struct Note{
  int enabled = 0;
  int freq = 0;
  int val = 0;
  float maxC = 0;
};

struct Speaker{
  int playing = -1;
  Note notes[2];
};
const int segCount = 512;
float sinVals[segCount];
Speaker speakers[maxSpeakers];
float degToRad(float d){
  return (d*PI)/180;
}
float testVals[] = {0,1,.5,.6,.33,.23,.543,.1236,.234,.695,.897};
float mySin(float v){
  int index = int(v*segCount);
  return sinVals[index];
  
}
void setup() {
    Serial.begin(9600);  

    for (int i=0;i<segCount;i++){
      sinVals[i] = sin((float(i)/segCount)*2*PI);
    }
    Serial.println(U_FREQ);
    
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));
    
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);
    
    TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
    TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
    TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    OCR2A = 127;
     
    cli();
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
    OCR1A = U_FREQ;

    /*
    //TIMSK1 |= _BV(OCIE1A);
  
    //TIMSK1 |= (1 << OCIE1A);
    //TIMSK1 |= (0 << OCIE1A);

    //TCCR0A = 0;
    //TCCR0B = 0;
    //TCNT0 = timer_counter[0];

    //TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00); 
    */
    sei();
    for (int i=0;i<sizeof(buzzers)/sizeof(int);i++){
      pinMode(buzzers[i], OUTPUT);
    }

}

/*
ISR(TIMER1_OVF_vect){
  int s = 0;
  int p = speakers[s].playing;
  if (p == -1){
    if (speakers[s].notes[0].enabled == 1){
      speakers[s].playing = 0;
    } else if (speakers[s].notes[1].enabled == 1){
      speakers[s].playing = 1;
    } else{
      speakers[s].playing = -1;
      digitalWrite(buzzers[s], LOW);
      return;
    }
  }
  
  p = speakers[s].playing;
  speakers[s].notes[p].count++;
 //Finished notes cycle
 if (speakers[s].notes[p].count > speakers[s].notes[p].maxCount){
  speakers[s].notes[p].count = 0;
  if (speakers[s].notes[(p-1)*-1].enabled == 1){
    digitalWrite(buzzers[s], LOW);
    speakers[s].playing = (p-1)*-1;
    TCNT1 = speakers[s].notes[(p-1)*-1].timer;
    return;
  }
 }
 digitalWrite(buzzers[s], digitalRead(buzzers[s])^ 1);
 p = speakers[s].playing;
 TCNT1 = speakers[s].notes[p].timer;

}*/

ISR(TIMER1_COMPA_vect){
  int count = 0;
  float sinVal = 0;
  if (speakers[0].notes[0].enabled == true){
    int v = speakers[0].notes[0].val++;
    float prog = (v/speakers[0].notes[0].maxC);
    sinVal += mySin(prog-int(prog));
    count++;
  }
  if (speakers[0].notes[1].enabled == true){
    int v = speakers[0].notes[1].val++;
    float prog = (v/speakers[0].notes[1].maxC);
    sinVal += mySin(prog-int(prog));
    count++;
  }
  if (count >= 1){
    int vol = (sinVal+count)*(127/count);
    OCR2A = vol;
  } else{
    OCR2A = 0;
  }
  /*int v = speakers[0].notes[0].val++;
  float prog = (v/speakers[0].notes[0].maxC);
  int vol = (mySin(prog-int(prog))+1)*127;
  */
  /*
  if (vol > 0.5){
    analogWrite(buzzers[0], 255);
  } else{
    analogWrite(buzzers[0], 0);
  }
  */
  //analogWrite(buzzers[0], vol);
  //OCR2A = vol;

  
  

}

void loop() {
  return;
  enable(0,440);
  delay(1000);
  enable(5, 659);
  delay(1000);
  disable(0);
  delay(1000);
  disable(5);
  delay(1000);
  
  /*
  tone(buzzers[1],420);
  delay(32000);
  noTone(buzzers[1]);
  delay(32000);
  */
  
  return;
}
void enable(int speaker, int freq){
  freq = freq;
  int note = int(speaker/5);

  if (speaker%5< 4){
    
    speakers[speaker%5].notes[note].freq = freq;
    speakers[speaker%5].notes[note].val = 0;
    speakers[speaker%5].notes[note].enabled = 1;
    speakers[speaker%5].notes[note].maxC = U_HZ/float(freq);
  }
  if (speaker%5 == 0){
    //TIMSK1 |= (1<<TOIE1);
    TIMSK1 |= _BV(OCIE1A);
    if (speakers[speaker%5].notes[(note-1)*-1].enabled == 0){
      //TCNT1 = speakers[speaker%5].notes[note].timer;
    }
  } else if (speaker%5 == 1){
    TIMSK3 |= (1<<TOIE3);
    if (speakers[speaker%5].notes[(note-1)*-1].enabled == 0){
      //TCNT3 = speakers[speaker%5].notes[note].timer;
    }
  } else if (speaker%5 == 2){
    TIMSK4 |= (1<<TOIE4);
    if (speakers[speaker%5].notes[(note-1)*-1].enabled == 0){
      //TCNT4 = speakers[speaker%5].notes[note].timer;
    }
  } else if (speaker%5 == 3){
    TIMSK5 |= (1<<TOIE5);
    if (speakers[speaker%5].notes[(note-1)*-1].enabled == 0){
      //TCNT5 = speakers[speaker%5].notes[note].timer;
    }
  } else if (speaker%5 == 4){
    tone(buzzers[speaker], freq);
  }

}
void disable(int speaker){
 int note = int(speaker/5);
 if (speaker%5< 4){
  speakers[speaker%5].notes[note].enabled = 0;
  speakers[speaker%5].notes[note].freq = 0;
 }
 if (speakers[speaker%5].notes[0].enabled == 0 && speakers[speaker%5].notes[1].enabled == 0){
  speakers[speaker%5].playing = -1;
   if (speaker%5 == 0){
      //TIMSK1 |= (0 << OCIE1A);
      //TIMSK1 = 0;

      TIMSK1 &= ~_BV(OCIE1A);

      digitalWrite(buzzers[0], LOW);
    } else if (speaker%5 == 1){
      TIMSK3 = 0;
      digitalWrite(buzzers[1], LOW);
    } else if (speaker%5 == 2){
      TIMSK4 = 0;
      digitalWrite(buzzers[2], LOW);
    } else if (speaker%5 == 3){
      TIMSK5 = 0;
      digitalWrite(buzzers[3], LOW);
    }else if (speaker%5 == 4){
      noTone(buzzers[speaker]);
    }
 } else{
  speakers[speaker%5].playing = (note-1)*-1;
 }
  

}
