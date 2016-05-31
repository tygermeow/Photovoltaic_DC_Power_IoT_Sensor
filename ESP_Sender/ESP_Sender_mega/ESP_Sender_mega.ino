
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int payLoad[2];
  Wire.requestFrom(5,4);
  if(Wire.available() == 4){

    for (int i=0; i<3; i++) payLoad[i] = Wire.read();
  
    int p = (payLoad[0] << 8) + payLoad[1];
    Serial.println(p);
    delay(1000);
  }
}
