// SELPIN  10   //Selection Pin (CS)
// DATAOUT 11   //MOSI 
// DATAIN  12   //MISO 
// SPICLOCK 13  //Clock

#include "AH_MCP320x.h"
#include <SPI.h>

//uint16_t analogValue = 0;
float time_in;
float time_out;
int ADCValues[8];                //definition of Array for measured values

//AH_MCP320x ADC_SPI(10);           //Object definition with D9 as CS, other pins are defined by spi interface (faster mode).
AH_MCP320x ADC_pin(10,11,12,13);    //Object definition with free definition of communication pins (slower mode).

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  time_in = micros();
//  ADC_SPI.readALL(ADCValues,8);    //read all 8 channels (SPI mode)
  ADC_pin.readALL(ADCValues,8);    //read all 8 channels (Pin mode)  
  time_out = micros();
  
  for (int i=0;i<8;i++)
  {
    Serial.print("CH"); 
    Serial.print(i);     
    Serial.print(": ");         
    Serial.print(ADCValues[i]);    
    Serial.print(" \t\t");
 // Serial.print(ADC_SPI.calcVOLT(5.0, ADCValues[i]));        //calculate input voltage at 5V VREF
    Serial.print(ADC_pin.calcVOLT(5.0, ADCValues[i]));    
    Serial.println("V");   
  }
  
  Serial.print("Time left: ");    
  Serial.print(time_out-time_in);    
  Serial.println(" ms");
  
  delay (2000);
  
  Serial.println();
  Serial.println();
}


