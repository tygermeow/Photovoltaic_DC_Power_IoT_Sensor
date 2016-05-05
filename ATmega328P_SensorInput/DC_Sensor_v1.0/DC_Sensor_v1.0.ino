#include <Wire.h>
#include "pins.h"

double voltage, current, power;
double accumulate_power, avg_power;

void setup() {
  //setup device as I2c Slave device 9
  Wire.begin(9);

  //setup pins
  pinMode(VOLTAGE, INPUT);
  pinMode(CURRENT, INPUT);
  
}

void loop() {
  //take an average reading of voltage, current, and time then report the
  //average power for that period of one second
  
  int i;
  accumulate_power = 0;

  for (i=0; i<1000; i++)
  {
    voltage = analogRead(VOLTAGE);  //this will read the value off of the voltage divider circuit
    voltage = map(voltage, 0, 1023, 0, 4000);  //set the maximum input to 40V
    current = analogRead(CURRENT) - DC_CURRENT_OFFSET;
    current = map(current, 0, 1023, 0, 100000);  //max current to 100A? need to calibrate this for
                                                 //specific sensor types
    #ifdef DEBUG
      Serial.print("Voltage : ");
      Serial.println(voltage);
      Serial.print("Current : ");
      Serial.println(current);
    #endif
    accumulate_power = accumulate_power + (current*voltage);  
    delay(1);
  }
  avg_power = (accumulate_power/1000000000);  //calculate the average power from sum of timesteps

  Wire.onRequest(send_avg_power);  //send data point
  

}

void send_avg_power()
  {
    Wire.write((int)avg_power);
  }
