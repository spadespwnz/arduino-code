#include <Servo.h>

Servo servo;
#define servoPin 9
int pos = 145;
void setup() 
{
    servo.attach(servoPin);
    Serial.begin(9600);     // communication with the host computer
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

    
}
 
void loop() 
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    /*if ( Serial1.available() ) {
     int s = Serial1.read();
     if (s != -1){
       Serial.write( s );  
     }
      
     }
     */
    //if ( Serial1.available() )       {  Serial.write( Serial1.read() );  }
    if ( Serial.available() )       {  Serial1.write( Serial.read() );  }
    //return;
    if (Serial1.available()){
      if (Serial1.find("+IPD,"))
      {
        delay(1000);
        int connId = Serial1.read()-48;
        Serial1.find("a=");
        int angle = (Serial1.read()-48);
        while (Serial1.available()){
          int a = Serial1.read()-48;
          if (a >= 0 && a < 10){
            angle = angle * 10;
            angle = angle + a;
          } else{

            break;
          }
        }
       
        pos = angle;
        Serial.println("New Pos: "+pos);
        sendPage(connId, angle);
        while (Serial1.available()){
          Serial1.read();
          
        }
      }
      
    }
    
    servo.write(pos);
    delay(10);
    
}
void sendPage(int id, int angle){
  String content = String(angle);
  String cipSend = "AT+CIPSEND=";
  cipSend += id;
  cipSend +=",";
  cipSend += content.length();
  cipSend += "\r\n";
  sendMsg(cipSend,1000);
  sendMsg(content,1000);
  String cipClose = "AT+CIPCLOSE=";
  cipClose += id;
  cipClose += "\r\n";
  sendMsg(cipClose, 1000);
}
String sendMsg(String msg, float d){
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

