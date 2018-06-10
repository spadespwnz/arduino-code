#include <binary.h>
#include <LedControl.h>
#define DIN 2
#define CS 3
#define CLK 4

LedControl lc = LedControl(2,4,3,1);
void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
}

void loop() {

  for (int i=0;i<8;i++){
    for (int j=0;j<8;j++){
      lc.setLed(0,i,j,true);
      delay(20);
    }
  }
  lc.clearDisplay(0);
  for (int i=0;i<8;i++){
    lc.setRow(0,i,B11111111);
    delay(100);
    lc.setRow(0,i,B0000000);
    
  }
}
