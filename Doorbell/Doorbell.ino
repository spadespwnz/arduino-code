#define button 7

int clockPin = 5;
int latchPin = 3;
int dataPin = 2;
byte leds = 0;

int buzzers[] = {4,10,11,12,13};
int buzzer_states[] = {0,0,0,0,0};
int timer_counter[] = {-1,-1,-1,-1,-1};
int timer_counter_r[] = {-1,-1,-1,-1,-1};
int buzzer_states2[] = {0,0,0,0,0};
int timer_counter2[] = {-1,-1,-1,-1,-1};
int timer_counter2_r[] = {-1,-1,-1,-1,-1};
int maxSpeakers = 9;
unsigned int tcnt2;
int buttonState = 0;
void setup() 
{
    Serial.begin(9600);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);  
    pinMode(clockPin, OUTPUT);
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    timer_counter[0] = 65536 - (16000000/256/329);
    TCNT1 = timer_counter[0];
    TCCR1B |= (1 << CS12);
  
    TCCR3A = 0;
    TCCR3B = 0;
    timer_counter[1] = 65536 - (16000000/256/329);
    TCNT3 = timer_counter[1];
    TCCR3B |= (1 << CS12);

    TCCR4A = 0;
    TCCR4B = 0;
    timer_counter[2] = 65536 - (16000000/256/329);
    TCNT4 = timer_counter[2];
    TCCR4B |= (1 << CS12);

    TCCR5A = 0;
    TCCR5B = 0;
    timer_counter[3] = 65536 - (16000000/256/329);
    TCNT5 = timer_counter[3];
    TCCR5B |= (1 << CS12);
    //TIMSK3 |= (1 << TOIE1);
    interrupts();

    for (int i=0;i<sizeof(buzzers)/sizeof(int);i++){
      pinMode(buzzers[i], OUTPUT);
    }

    pinMode(button, INPUT_PULLUP);

    Serial1.begin(9600);
    
    while (!Serial1){
    }

    
    delay(500);
    sendMsg("AT+RST\r\n",2000);
    sendMsg("AT+CWMODE=3\r\n",2000);
    sendMsg("AT+CWJAP=\"SHAW-953C30\",\"25117B075371\"\r\n",5000);
    sendMsg("AT+CIPMUX=1\r\n",2000);
    sendMsg("AT+CIPSERVER=1,80\r\n",2000);
    //sendMsg("AT+CWSAP=\"lul ur bad\", \"1234\",11,3\r\n",2000);
    delay(1000);
    
    
    //Could be used to setup Timer2 for interupt, instead using Timer2 for tone
    /*TIMSK2 &= ~(1<<TOIE2);
    TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
    TCCR2B &= ~(1<<WGM22);
    ASSR &= ~(1<<AS2);
    TIMSK2 &= ~(1<<OCIE2A);
    TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
    TCCR2B &= ~(1<<CS21);             // Clear bit
    tcnt2 = 256 - (tickInterval/8);
    TCNT2 = tcnt2;
    TIMSK2 |= (1<<TOIE2);*/
}


ISR(TIMER1_OVF_vect){
  TCNT1 = timer_counter[0];
  digitalWrite(buzzers[0], digitalRead(buzzers[0]) ^ 1);
}

ISR(TIMER3_OVF_vect){
  TCNT3 = timer_counter[1];
  digitalWrite(buzzers[1], digitalRead(buzzers[1]) ^ 1);
}

ISR(TIMER4_OVF_vect){
  TCNT4 = timer_counter[2];
  digitalWrite(buzzers[2], digitalRead(buzzers[2]) ^ 1);
}


ISR(TIMER5_OVF_vect){
  TCNT5 = timer_counter[3];
  digitalWrite(buzzers[3], digitalRead(buzzers[3]) ^ 1);
}

void enable(int speaker, int freq){

  if (speaker < 5){
    timer_counter[speaker] = 65536 - (16000000/256/freq);
    timer_counter_r[speaker] = timer_counter[speaker];
    buzzer_states[speaker] = 0;
  } else{
    timer_counter2[speaker%5] = 65536 - (16000000/256/freq);
    timer_counter2_r[speaker%5] = timer_counter2[speaker%5];
    buzzer_states2[speaker%5] = 0;
  }
  if (speaker%5 == 0){
    TIMSK1 |= (1<<TOIE1);
  } else if (speaker%5 == 1){
    TIMSK3 |= (1<<TOIE3);
  } else if (speaker%5 == 2){
    TIMSK4 |= (1<<TOIE4);
  } else if (speaker%5 == 3){
    TIMSK5 |= (1<<TOIE5);
  } else if (speaker%5 == 4){
    tone(buzzers[speaker], freq);
  }
  if (speaker < 8){
    bitSet(leds, speaker);
    updateShiftRegister();
  }
}
void disable(int speaker){
 if (speaker < 5){
  timer_counter[speaker%5] = -1;
  buzzer_states[speaker] = 0;
 } else{
  timer_counter2[speaker%5] = -1;
  buzzer_states2[speaker%5] = 0;
 }
 if (timer_counter[speaker%5] == -1 && timer_counter2[speaker%5] == -1){
   if (speaker%5 == 0){
      TIMSK1 = 0;
    } else if (speaker%5 == 1){
      TIMSK3 = 0;
    } else if (speaker%5 == 2){
      TIMSK4 = 0;
    } else if (speaker%5 == 3){
      TIMSK5 = 0;
    }else if (speaker%5 == 4){
      noTone(buzzers[speaker]);
    }
 }
  
  if (speaker < 8){
    bitClear(leds, speaker);
    updateShiftRegister();
  }

  /*digitalWrite(buzzers[speaker], LOW);
   state[speaker] = 0;
  enabled[speaker] = 0;*/
}



void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

void loop() 
{


    int newState = digitalRead(button);
    if (newState != buttonState){
      if (newState == LOW){
        sendData(0);
      }
      buttonState = newState;
    }
    if ( Serial.available() )       {  Serial1.write( Serial.read() );  }
    if (Serial1.available()){
      
      if (Serial1.find("+IPD,"))
      {
      delay(20);
      int connId = Serial1.read()-48;
      if (Serial1.find(":")){
        char cmd[10];
        Serial1.readBytesUntil('.', cmd, 10);
        char* value = strtok(cmd, ",");
        int freq = atoi(value);
        value = strtok(0, ",");
        int volume = atoi(value);
        value = strtok(0, ",");
        int speaker = atoi(value);
        if (speaker >= maxSpeakers){
          return;
        }
        if (speaker < 9){
          if (volume == 0){
            //noTone(buzzers[speaker]);
            disable(speaker);
          } else{
            enable(speaker, freq);
            

          }
        }
       
       }
      }
    }
}


void sendData(int id){
  String content = "play\n";
  String cipSend = "AT+CIPSEND=";
  cipSend += id;
  cipSend +=",";
  cipSend += content.length();
  cipSend += "\r\n";
  sendMsg(cipSend,50);
  sendMsg(content,50);
  /*String cipClose = "AT+CIPCLOSE=";
  cipClose += id;
  cipClose += "\r\n";
  sendMsg(cipClose, 1000);*/
}
String sendMsg(String msg, float d){
  //Serial.println("Sending msg: "+msg);
  String res = "";
  Serial1.print(msg);
  delay(d);
  while (Serial1.available()){
    char c = Serial1.read();
    res += c;
  }
  Serial.print(res);
  return res;
}

