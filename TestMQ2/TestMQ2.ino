#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <Wire.h>

#define VIRTUAL_CHANNEL_TEMPER 3
#define VIRTUAL_CHANNEL_HUMID 4
// Uncomment whatever type you're using!

const char* ssid = "NOURHOUSE";
const char* password = "";

const char cy_username[] = "7aa0e9a0-1bfa-11e8-9f38-9fae3d42ebf0";
const char cy_password[] = "80559e8bb8325d8f08e2fd182685a89339624357";
const char cy_clientID[] = "bb91d230-1bfa-11e8-96c3-51e4789cd606";

WiFiClient client;
int smokeA0 = A0;
int smokeD0 = 16;
void setup(void){
  Serial.begin(115200);
 // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  //display.print("Connected to ");
  //display.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }


  Serial.println("HTTP server started");
  Cayenne.begin(cy_username, cy_password, cy_clientID, ssid, password);
  Serial.println("Cayenne Connection established");

  pinMode(smokeA0, INPUT);
  pinMode(smokeD0, INPUT);
}
void loop(void){
  Cayenne.loop();
  int analogSensor = analogRead(smokeA0);
  int val = digitalRead(smokeD0); 
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  Serial.print("Pin D0: ");
  Serial.println(val);
  // Checks if it has reached the threshold value
 
 delay(10000);
 //Cayenne.celsiusWrite(VIRTUAL_CHANNEL_TEMPER, analogSensor);
 //Cayenne.luxWrite(VIRTUAL_CHANNEL_HUMID, smokeD0);
  
}
