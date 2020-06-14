#include <WiFi.h>
#include <Wire.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
#define DHTTYPE DHT11 
uint8_t DHTPin = 4;
DHT dht(DHTPin, DHTTYPE); 
float Temperature;
float Humidity;
void send_event(const char *event);
char *ssid = "TurkTelekom_T3FC9";
const char* password = "wTMDauX3";
const char *host = "maker.ifttt.com";
const char *privateKey = "bv_hjmTaYrbKeFlQ1HUDd4U27ZT7BAaz6RcJAp0o_5P";
WiFiServer server(80);
String header;

void setup() {
Serial.begin(115200);
pinMode(DHTPin, INPUT);
dht.begin();
lcd.init();
lcd.backlight();
Serial.print("Connecting to Wifi Network");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("Successfully connected to WiFi.");
Serial.println("IP address of ESP32 is : ");
Serial.println(WiFi.localIP());
server.begin();
Serial.println("Server started");
}


void loop()
{
Temperature = dht.readTemperature(); 
Humidity = dht.readHumidity(); 
WiFiClient client = server.available();
if (client) 
{ 
Serial.println("Web Client connected ");
String request = client.readStringUntil('\r'); 
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 10");  // update the page after 10 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<style>html { font-family: Cairo; display: block; margin: 0px auto; text-align: center;color: #333333; background-color: ##f3ffee;}");
client.println("body{margin-top: 50px;}");
client.println("h1 {margin: 50px auto 30px; font-size: 50px;  text-align: center;}");
client.println(".side_adjust{display: inline-block;vertical-align: middle;position: relative;}");
client.println(".text1{font-weight: 180; padding-left: 15px; font-size: 50px; width: 170px; text-align: left; color: #3498db;}");
client.println(".data1{font-weight: 180; padding-left: 70px; font-size: 50px;color: #3498db;}");
client.println(".text2{font-weight: 180;padding-left: 15px; font-size: 50px; width: 170px; text-align: left; color: #ff6600;}");
client.println(".data2{font-weight: 180; padding-left: 120px; font-size: 50px;color: #ff6600;}");
client.println(".data{padding: 10px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div id=\"webpage\">");   
client.println("<h1>ESP32 Interfacing with DHT11</h1>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Humidity:</div>");
client.println("<div class=\"side_adjust data1\">");
client.print(Humidity);
client.println("<div class=\"side_adjust text1\">%</div>");
client.println("</div>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text2\">Temperature:</div>");
client.println("<div class=\"side_adjust data2\">");
client.print(Temperature);
client.println("<div class=\"side_adjust text2\">*C</div>");
client.println("</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
 delay(4000);
  if ( Temperature >= 20) {
 send_event("temp_event");
 lcd.setCursor(0, 0);
 lcd.print("High Value");
 delay(1000);
 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("Occured");
 delay(1000);
 lcd.clear(); 

        }
}
}
void send_event(const char *event)
{
  Serial.print("Connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      delay(50);
    }
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
}
