/*  SILCD / SP Prototype
 *  
 *  By: Team Ness
 *  Updated: 12/14/2015
 *  
 *  This codes is for the ESP8266, that acts as the communication 
 *  interface between microcontrollers and SSAN gateway. 
 *  For this protype, the ESP8266 acts as a server and listens
 *  for HTTP requests. 
 *  At the same time, it acts as the I2C master, which means that it 
 *  requests the current data from the power measuring ATmega, and 
 *  forwards control commands to the frequency measuring and 
 *  load controlling second ATmega. 
 *  Future improvements will include the request of operating mode
 *  of the load controlling ATmega. 
 *    
 *  
 */
// Include the required Wire library for I2C
#include <Wire.h>
#include <ESP8266WiFi.h>
#define SER_DEBUG 1             // For debugging
#define PAYLOAD_SIZE 4          // number of byters per message from power nano
/*    WiFi network to join      */ 
const char* ssid = "The Hague";
const char* password = "FrenchForces1@";
//const char* ssid = "WRTnode_Proto";
//const char* password = "12345678";
int nodePayload[PAYLOAD_SIZE];
int relayState = 0;
int freqBounds[] = {5850,6150};
int current = 0;
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
void setup() {
  
  if (SER_DEBUG) Serial.begin(9600);
  // I2C master on SDA = GPIO4, and SCL = GPIO2 
  Wire.begin(4,2); 
  // join network
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (SER_DEBUG) Serial.print(".");
  }
  if (SER_DEBUG){
  Serial.println("");
  Serial.println("WiFi connected");
  }
  
  server.begin();                              // Start the server
  if (SER_DEBUG) {
    Serial.println("Server started");           // info message
    Serial.println(WiFi.localIP());             // Print the IP address
  }
}
void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
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
      Serial.println("invalid request");
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
  client.println(s);
  client.println(t);
  client.println(u);
  
  delay(1);
  if (SER_DEBUG) Serial.println("Client disonnected");
  if (SER_DEBUG)  {
    Serial.print("Sending to node 9: \t");
    Serial.print(relayState);  
    Serial.print("\t");
    Serial.print(freqBounds[0]);
    Serial.print("\t");
    Serial.println(freqBounds[1]);  
  }  
  
  Wire.beginTransmission(9);            // transmit to device #9 (frequency nano)
  Wire.write(relayState);               // sends the relayState
  Wire.write((freqBounds[0] >> 8));     // msb of lower bound
  Wire.write((freqBounds[0] & 0xff));   // lsb of lower bound
  Wire.write((freqBounds[1] >> 8));     // msb of upper bound
  Wire.write((freqBounds[1] & 0xff));   // lsb of upper bound
  Wire.endTransmission();               // stop transmitting
  
  
  Wire.requestFrom(5, PAYLOAD_SIZE);
  if (Wire.available()==PAYLOAD_SIZE){
    for (int i = 0; i<PAYLOAD_SIZE; i++) {
      nodePayload[i] = Wire.read();
    }
    // combine two 8-bits to 16-bit current adc reading
    current = (nodePayload[0] << 8) + nodePayload[1];
    //current[1] = (nodePayload[2] << 8) + nodePayload[3];
    if (SER_DEBUG) {
      Serial.print("Receiving from node 5:");
      Serial.print("\t");
      Serial.print(current);
      Serial.println("----------------");
    }
  }
  
  
  delay(200);
}
