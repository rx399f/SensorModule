
// Import required libraries
#include "ESP8266WiFi.h"
#include <aREST.h>


#define HUM_ANALOG_PIN        2
#define HUM_DIGITAL_PIN       5
int analogReading;
int digitalIO;
const char* ssid = "$$OPTEAMIX$$";
const char* password = "1tV;n2y!<"; 
aREST rest = aREST();
WiFiServer server(80);
uint8_t  MAC_softAP[]          = {0,0,0,0,0,0};                      //not sure why we have to pass a MAC address to get a MAC address.
uint8_t  MAC_STA[]                = {0,0,0,0,0,0}; 

void setup()
{
  
  Serial.begin(115200);  

  Serial.println("Moisture Sensorn init");

  Serial.print("MAC[SoftAP]");
  uint8_t* MAC  = WiFi.softAPmacAddress(MAC_softAP);                   //get MAC address of softAP interface
  for (int i = 0; i < sizeof(MAC)+2; ++i){                                                          //this line needs cleaning up.
       Serial.print(":");
       Serial.print(MAC[i],HEX);
       MAC_softAP[i] = MAC[i];                                         //copy back to global variable
  }
  Serial.println();
  Serial.print("MAC[STA]");
  MAC  = WiFi.macAddress(MAC_STA);                   //get MAC address of STA interface
  for (int i = 0; i < sizeof(MAC)+2; ++i){
       Serial.print(":");
       Serial.print(MAC[i],HEX);
       MAC_STA[i] = MAC[i];                                            //copy back to global variable
  }
  Serial.println();

 
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  rest.variable("digitalMoistureLevel",&digitalIO);
  rest.variable("analogMoistureLevel",&analogReading);

  rest.set_id("1");
  rest.set_name("sensor_module");

  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{
  analogReading = analogRead(HUM_ANALOG_PIN);  
  // Serial.println(lastAnalogReading);
  digitalIO = digitalRead(HUM_DIGITAL_PIN);
  // Serial.println(digitalIO);
  // delay(5000);
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  
}



