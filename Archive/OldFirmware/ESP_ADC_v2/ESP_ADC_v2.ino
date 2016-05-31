// Include the required Wire library for I2C
//#include <Wire.h>
#include <ESP8266WiFi.h>
#include "AH_MCP320x.h"
#include <SPI.h>
#define SER_DEBUG 1             // For debugging
/*    WiFi network to join      */ 
const char* ssid = "The Hague";
const char* password = "FrenchForces1@";
const char* host="10.0.0.1";
//const char* ssid = "WRTnode_46";
//const char* password = "12345678";
//const char* host="192.168.8.1";
int plug = 777; 
int led = 2;
int voltage = 0;
int accumulate_voltage;
double current = 0;
double accumulate_current;
int power = 0;
int i = 0;
double accumulate_power = 0;
// AH_MCP320z(int CS_pin, int DOUT_pin, int DIN_pin, int CLK_pin)
AH_MCP320x ADC_pin(16, 14, 12, 13); 
// Create an instance of the server
// specify the port to listen on as an argument
//WiFiServer server(80);
WiFiClient client;




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
  voltage = ADC_pin.readCH(0);
  voltage = voltage*1.9239-17.399;  //convert to voltage in millivolts
  accumulate_voltage = accumulate_voltage + voltage;     //voltage divider for input is 1M and 90k
  delayMicroseconds(15);
  current = ADC_pin.readCH(1);                //ok calibrated
  //current = -(5.98/310)*current + 39.468;  //no OP-AMP w/ ACS712
  current = -0.05339285714285710000*current + 216.29;  //MCP3208 w TLV2460 op amp and 100k 100k feedback
  //current = -0.07673521850899750000*current + 157.04;  //MCP3208 no op amp
  accumulate_current = accumulate_current + current;
  accumulate_power = accumulate_power + voltage*current;
  i++;
  
  

  if (i > 15000) {
    digitalWrite(led, HIGH);
    double avg_power = accumulate_power/(i*1000);     //find the average power for the time period
    if (SER_DEBUG) {
      Serial.println(i);
      Serial.println(accumulate_power);
      Serial.print("ding");
      Serial.println("----------");
      Serial.println(avg_power);
      Serial.print("voltage: ");
      Serial.println(accumulate_voltage/i);
      Serial.print("current: ");
      Serial.println(accumulate_current/i);
    }
    accumulate_power=0;
    accumulate_voltage=0;
    accumulate_current=0;
    i=0;
    const int httpPort = 9000;
    if (!client.connect(host, httpPort)) {
      if (SER_DEBUG) Serial.println("connection failed");
      //return;
    }
  
    else {
      //some script to send power message with timestamp to server
      //for iobeam or our other server
        //
    }
  }
}
  
  /*
  const int httpPort = 9000;
  
  if (!client.connect(host, httpPort)) {
      if (SER_DEBUG) Serial.println("connection failed");
      //return;
  }
  else {
    String url = "/Type?1";
    url += "/Current?";
    url += String(current);
    url += "/State?";
    url += String(relayState);
  
    client.print(String("GET "+url+" HTTP/1.1\r\n"+
                "Host: "+host+" \r\n"+
                "Connection: close\r\n\r\n"));
    delay(50);
    while (client.available()){
      String line = client.readStringUntil('\r');
      if (SER_DEBUG) Serial.println(line);
      int Index[6];
      Index[0] = line.indexOf('?');   
      Index[1] = line.indexOf('#', Index[0]+1);           // state?1#
      Index[2] = line.indexOf('?', Index[1]+1);           
      Index[3] = line.indexOf('#', Index[2]+1);           // upper?6050#
      Index[4] = line.indexOf('?', Index[3]+1);
      Index[5] = line.indexOf('!', Index[4]+1);           // lower?5950!
      String firstString = line.substring(Index[0]+1, Index[1]);
      String secondString = line.substring(Index[2]+1, Index[3]);
      String thirdString = line.substring(Index[4]+1, Index[5]);
      if (SER_DEBUG){
        Serial.println(firstString);
        Serial.println(secondString);
        Serial.println(thirdString);
      }
      relayState = firstString.toInt();
      freqBounds[1] = secondString.toInt();
      freqBounds[0] = thirdString.toInt();
      if (SER_DEBUG) {
        Serial.print("Parsed String:  ");
        Serial.print("\t");
        Serial.print(relayState);
        Serial.print("\t");
        Serial.print(freqBounds[1]);
        Serial.print("\t");
        Serial.println(freqBounds[0]);         
      }
    }
    
  }
    
    // Check if a client has connected
  if (MANUAL_MODE) {
    client = server.available();
  
    // if client has connected and a request is available, update on/off, freq value
    
    if (client && client.available()) {
      
      String req = client.readStringUntil('\r');    // Read the first line of the request
      if (SER_DEBUG) Serial.println(req);  
      client.flush();
    
      // Match the request
      if (req.indexOf("/gpio/0") != -1)
        relayState = 0;
      else if (req.indexOf("/gpio/1") != -1)
        relayState = 1;
      else if (req.indexOf("/lower/?") != -1) {
        int ind1 = req.indexOf('?');
        String low = req.substring(ind1+1, ind1+5);
        freqBounds[0] = low.toInt();
        if (SER_DEBUG) {
          Serial.print("Lower Bound Input: \t");
          Serial.println(freqBounds[0]);
        }
      }
      else if (req.indexOf("/upper/?") != -1) {
        int ind1 = req.indexOf('?');
        String low = req.substring(ind1+1, ind1+5);
        freqBounds[1] = low.toInt();
        if (SER_DEBUG) {
          Serial.print("Upper Bound Input: \t");
          Serial.println(freqBounds[1]);
        }
      }
        
      
      else {
        if (SER_DEBUG) Serial.println("invalid request");
        //client.stop();
        //return;
      }
    }
  
    
    client.flush();
  
    // Prepare the response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
    s += (relayState)?"high":"low";
    String t = "\n Lower frequency bound [Hz]: ";
    t += String(freqBounds[0]);
    t += "\n Upper frequency bound [Hz]: ";
    t += String(freqBounds[1]);
    String u = "Circuit 1 [A/D count]: ";
    u += String(current);
    s += "</html>\n";
    
    // Send the response to the client
    client.println(String(s+t+u));
    
  }
  
  delay(100);
  if (SER_DEBUG) Serial.println("Client disonnected");
 
  delay(200);
}
*/
