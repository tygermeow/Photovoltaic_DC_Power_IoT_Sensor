/* This device collects voltage and current data from a DC source
 *  and starts accumulating power data.  When a request is sent by
 *  the master device, an average of the power during the intervening
 *  period is sent and the accumulators are reset.
 *  
 */
 
#include <Wire.h>
#include "pins.h"

float voltage, current, power;
float accumulate_power;  //Initialize accumulators
int sample_counter=0;

void setup() {
  //setup device as I2c Slave device 9
  Wire.begin(5);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  //setup pins
  pinMode(VOLTAGE, INPUT);
  pinMode(CURRENT, INPUT);
  Wire.onRequest(send_avg_power);  //send data point
  
}

void loop() {
  //take an average reading of voltage, current, and time then report the
  //average power for that period the program will count the number of samples
  //between requests, then divide to find the average for that period
  //the counter will be reset when data is sent.
    //read signals
    voltage = analogRead(VOLTAGE);  //this will read the value off of the voltage divider circuit
    voltage = map(voltage, 0, 1023, 0, 4100);  //set the maximum input to 40V this is set by voltage divider to be linear input
                                                //rated in millivolts
    current = analogRead(CURRENT)*(-6.75/89)+38.7556179775281;  //recalculate this value using calibration curve data
    
    #ifdef DEBUG
      Serial.print("Voltage : ");
      Serial.println(voltage);
      Serial.print("Current : ");
      Serial.println(current);
      delay(250);
    #endif
    accumulate_power = accumulate_power + (current*voltage); 
    sample_counter++; 
    delay(1);
  
 

}

void send_avg_power()
  {
    int avg_power;
    avg_power = abs(int(accumulate_power/sample_counter));  //calculate the average power from sum of timesteps
    int nodePayload[4];  
    nodePayload[0] = (avg_power >> 24);            // msb1, (x >>8 ) = ( x/256)
    nodePayload[1] = (avg_power >> 16);          // lsb1
    nodePayload[2] = (avg_power >> 8);            // msb1, (x >>8 ) = ( x/256)
    nodePayload[3] = (avg_power & 0xff);          // lsb1
    for (int i=0; i < 3; i++) Wire.write(nodePayload[i]);
    accumulate_power = 0;
    sample_counter = 0;     //reset accumulators
    #ifdef DEBUG
    Serial.println(avg_power);
    #endif
  }

