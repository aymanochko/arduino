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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#define OLED_RESET LED_BUILTIN  //

#define DHTPIN 14     // what digital pin we're connected to
#define VIRTUAL_CHANNEL_TEMPER 1
#define VIRTUAL_CHANNEL_HUMID 2
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
const char* ssid = "NOURHOUSE";
const char* password = "";

const char cy_username[] = "7aa0e9a0-1bfa-11e8-9f38-9fae3d42ebf0";
const char cy_password[] = "80559e8bb8325d8f08e2fd182685a89339624357";
const char cy_clientID[] = "bb91d230-1bfa-11e8-96c3-51e4789cd606";

String server = "api.openweathermap.org";
String apiKey = "3d07cfe914e2335a8f87ea4135975607";
String location = "TOURS,FR";
String currentWeather = "GET /data/2.5/weather?";

unsigned char wifi1_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000111, 0b11100000, //      ######     
  0b00011111, 0b11111000, //    ##########   
  0b00111111, 0b11111100, //   ############  
  0b01110000, 0b00001110, //  ###        ### 
  0b01100111, 0b11100110, //  ##  ######  ## 
  0b00001111, 0b11110000, //     ########    
  0b00011000, 0b00011000, //    ##      ##   
  0b00000011, 0b11000000, //       ####      
  0b00000111, 0b11100000, //      ######     
  0b00000100, 0b00100000, //      #    #     
  0b00000001, 0b10000000, //        ##       
  0b00000001, 0b10000000, //        ##       
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

//const int led = 13;


// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup(void){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  Serial.begin(115200);
 // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  dht.begin();

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
   drawWifiIcon();
  display.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }


  Serial.println("HTTP server started");
  display.display();
  Cayenne.begin(cy_username, cy_password, cy_clientID, ssid, password);
  Serial.println("Cayenne Connection established");
}
int i=0;
float h, t, hExt, tExt;
void loop(void){
  Cayenne.loop();
   // Wait a few seconds between measurements.
  delay(10000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  if(i==0){
    //getWeather();
  }
  i++;
  if(i==1000){
    i=0;
  }
  display.clearDisplay();
  display.setCursor(0,0);
  drawWifiIcon();
  display.println(WiFi.localIP());
  display.println();
  display.print("H: ");
  display.print((int)h);
  display.print(" % ");
  display.print("T: ");
  display.print((int)t);
  display.println(" C");
   display.print("H Ext: ");
  display.print((int)hExt);
  display.print(" % ");
  display.print("T Ext: ");
  display.print((int)tExt);
  display.println(" C");
  display.display();
 Cayenne.celsiusWrite(VIRTUAL_CHANNEL_TEMPER, t);
 Cayenne.luxWrite(VIRTUAL_CHANNEL_HUMID, h);
  
}

void drawWifiIcon(){
   display.drawBitmap(110, 0, wifi1_icon16x16, 16, 16, 1);
}
String result;

void getWeather() {

  Serial.println("\nStarting connection to server...");

    if (client.connect(server, 80))   
    {                                         //starts client connection, checks for connection
        client.println("GET /data/2.5/weather?q="+location+"&units=metric&APPID="+apiKey);
        client.println("Host: "+server);
        client.println("User-Agent: ArduinoWiFi/1.1");
        client.println("Connection: close");
        client.println();
    } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         result = result+c;
       }

client.stop();                                      //stop client
result.replace('[', ' ');
result.replace(']', ' ');
Serial.println(result);
char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';
StaticJsonBuffer<1024> json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);

if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

String location = root["name"];
String country = root["sys"]["country"];
float temperature = root["main"]["temp"];
float humidity = root["main"]["humidity"];
String weather = root["weather"]["main"];
String description = root["weather"]["description"];
float pressure = root["main"]["pressure"];
tExt=temperature;
hExt=humidity;
}
