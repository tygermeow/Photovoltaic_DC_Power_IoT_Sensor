// SELPIN   10 //Selection Pin (CS)
// DATAOUT  11 //MOSI 
// DATAIN   12 //MISO 
// SPICLOCK 13 //Clock

#include "AH_MCP320x.h"
#include <SPI.h>

uint16_t analogValue = 0;
float time_in;
float time_out;
int analogValues[4];           //definition of Array for measured values

AH_MCP320x ADC_SPI(10);         //Objekt definition with D9 as CS, other pins are defined by spi interface (faster mode).
//AH_MCP320x ADC_pin(10,11,12,13);  //Objekt definition with free definition of communication pins (slower mode).

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  time_in = micros();
  ADC_SPI.readALL(analogValues,4);    //read all 4 channels (SPI mode)
//ADC_pin.readALL(analogValues,4);    //read all 4 channels (Pin mode)  
  time_out = micros();
  
  for (int i=0;i<4;i++)
  {
    Serial.print("CH"); 
    Serial.print(i);     
    Serial.print(": ");         
    Serial.print(analogValues[i]);    
    Serial.print(" \t\t");
    Serial.print(ADC_SPI.calcVOLT(5.0,analogValues[i]));          //calculate input voltage value at 5V Vref
   // Serial.println(ADC_pin.calcVOLT(5.0,analogValues[i]));    
    Serial.println("V");   
  }
  
  Serial.print("Time left: ");    
  Serial.print(time_out-time_in);    
  Serial.println(" ms");
  delay (2000);
}


