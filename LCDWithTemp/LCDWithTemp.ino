#include <LiquidCrystal.h>
#define tempPin 0
const int rs = 7, en = 8, d4 = 9, d5  = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.print("Current Temp:");
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp = analogRead(tempPin);
  double tempK = log(10000.0 * ((1024.0 / temp - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;
  float tempF = (tempC * 9.0) / 5.0 + 32.0;
  lcd.setCursor(0,1);
  lcd.print(tempC);
  delay(500);
}
