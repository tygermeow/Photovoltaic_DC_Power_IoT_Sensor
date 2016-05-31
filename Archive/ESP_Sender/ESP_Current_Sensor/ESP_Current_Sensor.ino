/* This code is an alternative method for capturing DC power from
 *  PV panels.  There is a 328P which will capture the DC voltage and
 *  send values via I2C.  This device will capture the DC current through
 *  hall effect transducer via the ADC pin.  The transducer has an offset
 *  of 2.34V and I was having trouble with the op amp that we had available
 *  so alternatively I can just voltage divide between 2.34 and 0V for the 
 *  current range to get the full 10 bit resolution between 0 and 1 V.This is
 *  a voltage divider with a 1M and 746k resistor.
 */
#define DEBUG 1
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Wire.h>

double current = 0;  //storage space for reading current through ADC
int   voltage = 0;  //voltage will come in through I2C
double power = 0;   //likewise for power

void setup() {
if(DEBUG) Serial.begin(9600);
Wire.begin();     //setup default pins 2 (SDL) 14 (SDA)
}

void loop() {

  //********************POWER ACQUISITION**************************
  int incoming_voltage[2];
  current = analogRead(A0);                 //read the current signal off of the ADC pin
  current = map(current, 0, 1023, 30, 0);  //I'll replace this later with a calibrated curve

  Wire.requestFrom(5,2);    //request the 2 bytes of data off of the ADC from 328P partner MC
  if(Wire.available()==2) 
  {
    for(int i=0; i<3; i++) incoming_voltage[i]=Wire.read();
    voltage = (incoming_voltage[0]<<8) + incoming_voltage[1];
    voltage = map(voltage, 0, 1023, 0, 40000);      //set voltage in millivolts
    if(DEBUG) Serial.println(voltage);
  }
  power = current*voltage/1000;     //calculate power
  if(DEBUG) Serial.println(power);
  //*****************************************************************
}
