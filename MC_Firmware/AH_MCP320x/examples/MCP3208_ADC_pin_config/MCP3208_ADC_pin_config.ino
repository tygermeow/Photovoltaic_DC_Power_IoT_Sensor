// SELPIN 10     //Selection Pin (CS)
// DATAOUT 11    //MOSI 
// DATAIN  12    //MISO 
// SPICLOCK  13  //Clock

#include "AH_MCP320x.h"
#include <SPI.h>

int ADCValue = 0;
float time_in;
float time_out;

//AH_MCP320x ADC_SPI(10);         //Object definition with D9 as CS, other pins are defined by spi interface (faster mode).
AH_MCP320x ADC_pin(10,11,12,13);  //Object definition with free definition of communication pins (slower mode).

void setup()
{
  Serial.begin(57600);
  Serial.println("Serial Ready");
}

void loop()
{
  ADC_pin.setCONFIG(0,SINGLE);                    //CH0 set single mode
  ADC_pin.setCONFIG(1,SINGLE);                    //CH1 set single mode
  
  ADCValue = ADC_pin.readCH(0);                   //read CH0 value
  Serial.print ("CH0: ");
  Serial.print(ADC_pin.calcVOLT(5.0,ADCValue));   //calculate the input voltage
  Serial.println("V");
  
  ADCValue = ADC_pin.readCH(1);                   //read CH1 value  
  Serial.print ("CH1: ");
  Serial.print(ADC_pin.calcVOLT(5.0,ADCValue));   //calculate the input voltage  
  Serial.println("V");
 
  ADC_pin.setCONFIG(0,DIFFERENTIAL);              //set CH0-CH1 differential mode
  ADCValue = ADC_pin.readCH(0);                   //read CH1 value  
  Serial.print ("CH0-CH1: ");
  Serial.print(ADC_pin.calcVOLT(5.0,ADCValue));   //calculate the input voltage  
  Serial.println("V");
  
  
  ADC_pin.setCONFIG(1,DIFFERENTIAL);              //set CH1-CH0 differential mode
  ADCValue = ADC_pin.readCH(1);                   //read CH1 value  
  Serial.print ("CH1-CH0: ");
  Serial.print(ADC_pin.calcVOLT(5.0,ADCValue));   //calculate the input voltage  
  Serial.println("V");
  
  delay (2000);
  Serial.println();
}


