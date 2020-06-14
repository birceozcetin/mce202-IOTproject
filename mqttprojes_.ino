#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define WLAN_SSID       "TurkTelekom_T3FC9"
#define WLAN_PASS       "wTMDauX3"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME  "bb123"
#define AIO_KEY       "aio_mTkK642GoJo5U7A7bQcDyEd0gG4h"
const int sol_i = 2; 
const int sol_g = 12;
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/mqtt-test.onoff");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(sol_i, OUTPUT); 
  pinMode(sol_g, OUTPUT);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&onoffbutton);
}

uint32_t x=0;

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
     
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      if (!strcmp((char*)onoffbutton.lastread, "ON")) {
        
    digitalWrite(sol_i , HIGH);
    digitalWrite(sol_g ,  LOW);
      }
      else {
       
    digitalWrite(sol_i , LOW);
    digitalWrite(sol_g ,  LOW);
      }   } }
     
}

void MQTT_connect() {
  int8_t ret;
 if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000); 
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
