// SELPIN 10    //Selection Pin (CS)
// DATAOUT 11   //MOSI 
// DATAIN  12   //MISO 
// SPICLOCK  13 //Clock

#include "AH_MCP320x.h"
#include <SPI.h>

int ADCValue = 0;
float time_in;
float time_out;
int analogValue;

//AH_MCP320x ADC_SPI(10);             //Object definition with D9 as CS, other pins are defined by spi interface (faster mode).
AH_MCP320x ADC_pin(10,11,12,13);  //Object definition with free definition of communication pins (slower mode).

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  for (int i=0;i<4;i++){  
    Serial.print("CH"); 
    Serial.print(i);     
    Serial.print(": ");   
    time_in = micros();  
    analogValue = ADC_SPI.readCH(i);    //read analog value from IC (SPI mode)
//    analogValue = ADC_pin.readCH(i);    //read analog value from IC (Pin mode)   
    time_out = micros();
    
    Serial.println(analogValue);   
    
    Serial.print("Voltage:");
    Serial.print(ADC_SPI.calcVOLT(3.3,analogValue));   //calculate the input voltage
//    Serial.print(ADC_pin.calcVOLT(3.3,analogValue));   //calculate the input voltage
    Serial.println(" V");
    
    Serial.print("Time: ");     
    Serial.print(time_out-time_in);   
    Serial.println(" ms");     
    Serial.println();
  
    delay(10);
  }
  delay (1000);
}


