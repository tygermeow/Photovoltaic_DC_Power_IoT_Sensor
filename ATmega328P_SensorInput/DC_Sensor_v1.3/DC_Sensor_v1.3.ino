#include <Wire.h>
#include "pins.h"

double voltage, current, power;
double accumulate_power;  //Initialize accumulators
int sample_counter=0;

void setup() {
  //setup device as I2c Slave device 9
  Wire.begin(9);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  //setup pins
  pinMode(VOLTAGE, INPUT);
  pinMode(CURRENT, INPUT);
  
}

void loop() {
  //take an average reading of voltage, current, and time then report the
  //average power for that period the program will count the number of samples
  //between requests, then divide to find the average for that period
  //the counter will be reset when data is sent.
    //read signals
    voltage = analogRead(VOLTAGE);  //this will read the value off of the voltage divider circuit
    voltage = map(voltage, 0, 1023, 0, 4000);  //set the maximum input to 40V this is set by voltage divider to be linear input
                                                //rated in millivolts
    current = analogRead(CURRENT);  //recalculate this value using calibration curve data
    
    #ifdef DEBUG
      Serial.print("Voltage : ");
      Serial.println(voltage);
      Serial.print("Current : ");
      Serial.println(current);
    #endif
    accumulate_power = accumulate_power + (current*voltage); 
    sample_counter++; 
    delay(1);
  
  Wire.onRequest(send_avg_power);  //send data point
  

}

void send_avg_power()
  {
    double avg_power;
    avg_power = (accumulate_power/sample_counter);  //calculate the average power from sum of timesteps

    Wire.write((int)avg_power);
    accumulate_power = 0;
    sample_counter = 0;     //reset accumulators
  }

