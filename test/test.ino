void setup() {
  Serial.begin(9600);     
  int h = 220;
  int c = (((float(1.0/h)*1000)/2)/64)*1000;
  Serial.print(c, DEC);

}

void loop() {
  // put your main code here, to run repeatedly:

}
