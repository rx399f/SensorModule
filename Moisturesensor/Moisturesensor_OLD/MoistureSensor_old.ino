 
// Import required libraries
#include "ESP8266WiFi.h"
#include <aREST.h>


#define HUM_ANALOG_PIN        2
#define HUM_DIGITAL_PIN       5
int analogReading;
int digitalIO;
const char* ssid = "Rafi's iPhone";
const char* password = "9980526238"; 
aREST rest = aREST();
WiFiServer server(80);
uint8_t MAC_array[6];
char MAC_char[18];

void setup()
{
  
  Serial.begin(115200);  

  Serial.println("Moisture Sensorn init");
  WiFi.macAddress(MAC_array);
  for (int i = 0; i < sizeof(MAC_array); ++i){
    Serial.print(MAC_char,"%s%02x:",MAC_char,MAC_array[i]); 
 
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



