/*This code is for the ESP8266 board.  It receives an average power
 * in milliwatts upon request from a sister DC_Sensor microporcessor.
 * The two way communication is achieved via I2C between microcontrollers
 * to pass the sensor data.  Messages are then sent via WiFi to the wireless
 * gateway device.
 */
#include <Wire.h>
#include <ESP8266WiFi.h>
#define SER_DEBUG 1
#define DEBUG
/*    WiFi network to join      */ 
//const char* ssid = "The Hague";
//const char* password = "FrenchForces1@";
const char* ssid = "WRTnode_46";
const char* password = "12345678";
const char* host="192.168.8.1";
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
int power=0;

void setup() {
  Wire.begin(2,5);     //setup default pins 2 (SDL) 14 (SDA)
  #ifdef DEBUG
   Serial.begin(9600);
  #endif
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
  // put your main code here, to run repeatedly:
  int payLoad[2];
  Wire.requestFrom(5,4);
  if(Wire.available()){
    Serial.println("receiving");
    for (int i=0; i<3; i++) payLoad[i] = Wire.read();
    p = (payLoad[0] << 8) + payLoad[1];
    Serial.println(p);
    delay(1000);
  }
  WiFiClient client;
  const int httpPort = 9000;  
  if (!client.connect(host, httpPort)) {
      if (SER_DEBUG) Serial.println("connection failed");
      //return;
  }
      // Prepare the response
    s = String(p)
    
    // Send the response to the client
    client.println(String(s));
  
}
