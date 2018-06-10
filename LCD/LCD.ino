#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 9, d5  = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.print("Hello, world!");
  Serial.begin(9600);
}

void loop() {
  lcd.autoscroll();
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,1);
  lcd.print(millis()/1000);
  delay(200);
  if (Serial.available()){ 
     delay(100);
     lcd.clear();
     while (Serial.available() > 0){
      lcd.write(Serial.read());
     }
  }
}
