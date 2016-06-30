
#include "AH_MCP320x.h"
#include <SPI.h>



//AH_MCP320x ADC_SPI(53);         //Object definition with D53 as CS, other pins are defined by spi interface (faster mode).
AH_MCP320x ADC_pin(53,51,50,52);  //MEGA pin mode
int configArray[8];

void setup()
{
  Serial.begin(57600);
  Serial.println("Serial Ready");
}

void loop()
{
  ADC_pin.getCONFIG(configArray,8);
  for(int i;i<8;i++)
  {
    Serial.print("CH:");
    Serial.print(i);
    Serial.print(" Mode:");
    Serial.println(configArray[i]); 
  }
  Serial.println();  
  delay (2000);
  
  if  (configArray[0]){
    Serial.println("Set all differential");
    ADC_pin.setCONFIG_allDiff(8);
  }
  else{
    Serial.println("Set all single");
    ADC_pin.setCONFIG_allSingle(8);    
  }
  

}


