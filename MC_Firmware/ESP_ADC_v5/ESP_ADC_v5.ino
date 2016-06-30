/*	DC Energy Meter
 *	
 *	By: Matsu Thornton
 *	Updated: 06/25/2016
 * 
 *	DC energy meter for photovoltaic installations using DC hall-effect sensor.
 *
 */

/*		libraries					*/
#include <ESP8266WiFi.h>
#include "AH_MCP320x.h"
#include <SPI.h>

/*		debugging					*/
#define SER_DEBUG 1             

/*	    WiFi network to join		*/
const char* ssid = "WRTnode_Proto";
const char* password = "12345678";
const char* host="192.168.8.1";
const int httpPort = 9000;

/* create an instance of the client	*/
WiFiClient client;

/*		DRMS definition				*/
int plug = 8;

/*		constants					*/ 
int led = 2;

/*		variables					*/
//int voltage = 0;
//int accumulate_voltage;
//double current = 0;
//double accumulate_current;
//int power = 0;
//int i = 0;
//double accumulate_power = 0;

/*		MCP308 ADC					*/
// AH_MCP320z(int CS_pin, int DOUT_pin, int DIN_pin, int CLK_pin)
AH_MCP320x ADC_pin(16, 14, 12, 13); 

void setup() {
  
  if (SER_DEBUG) Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (SER_DEBUG) Serial.print(".");
  }
  if (SER_DEBUG){
  Serial.println("");
  Serial.println("WiFi connected");
  }
  
  //server.begin();                              // Start the server
  //digitalWrite(led, HIGH);
  //if (SER_DEBUG) {
  //  Serial.println("Server started");           // info message
  //  Serial.println(WiFi.localIP());             // Print the IP address
  //}
}


void loop() {

  int powerValue = calculatePower();

  sendData(powerValue);

}

int calculatePower(){
  digitalWrite(led, LOW);
  /*	1 second of data acquisition	*/ 
	int iter = 0;
	float accumulate_voltage = 0;
	float accumulate_current = 0;
	float accumulate_power = 0;	
	while (iter < 15000) {
		/*	Get Voltage				*/ 
		float voltage = ADC_pin.readCH(0) * 0.0170969993021633*100-26.78;	// read pin and convert to something
		accumulate_voltage = accumulate_voltage + voltage;		//voltage divider for input is 1M and 90k
		delayMicroseconds(15);

		/*	Get Current				*/
		int current = -0.078974358974359*ADC_pin.readCH(1)+ 161.34;                //ok calibrated
		accumulate_current = accumulate_current + current;
		
		/*	Calculate accumulate power	*/
		accumulate_power = accumulate_power + voltage*current;

		/*	Update iterations						*/ 
		iter++;
  }

	/* 	Signal that data acquisition finished 	*/  
	digitalWrite(led, HIGH);
	
	/* 	calculate power and convert to int			*/
	int avg_power = int (accumulate_power/(iter*100.0) * 100.0);
  if (avg_power < 1000) avg_power = 0;
	
	/* 	print debugs									*/	
	if (SER_DEBUG) {
      Serial.println(accumulate_power);
      Serial.print("ding");
      Serial.println("----------");
      Serial.println(avg_power);
      Serial.print("voltage: ");
      Serial.println(accumulate_voltage/iter);
      Serial.print("current: ");
      Serial.println(accumulate_current/iter);
  }
  
	/* return average power 		*/
	return avg_power;
}

void sendData(int value){

	/* try connecting to hose 	*/ 
  if (!client.connect(host, httpPort)) {
      if (SER_DEBUG) Serial.println("connection failed");
      return;
  }

	/* if connect, send string to WRTnode */ 
  else {
    String url = "/Type?";
		url += String(plug);
    url += "/Current?";
    url += String(value);
    url += "/State?";
    url += String(1);
  
    client.print(String("GET "+url+" HTTP/1.1\r\n"+
                "Host: "+host+" \r\n"+
                "Connection: close\r\n\r\n"));
    delay(50);
    
  }
 
 	client.flush();
  
}

