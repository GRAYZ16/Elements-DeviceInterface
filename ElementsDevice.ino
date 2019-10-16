#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include "Elements.h"

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/**WIFI SETUP****************************/
#define WLAN_SSID "ssid"
#define WLAN_PASS "pass"

/**MQTT SETUP****************************/
#define MQTT_SERVER "192.168.0.3"
#define MQTT_PORT   1883
#define MQTT_USER   "pi"
#define MQTT_PASS   "pass"

/**CONN SETUP****************************/
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASS);

/**ELEMENTS SETUP************************/
#define DEVICE_NAME "Test Elements Device"
#define DEVICE_DESC "Example Temp/Humidity Device"
#define DEVICE_NO_OF_SENSORS 2


void setup() 
{
  Serial.begin(9600);
  while(!Serial);

  String deviceName = DEVICE_NAME;
  String deviceDesc = DEVICE_DESC;
  uint8_t deviceNoOfSensors = DEVICE_NO_OF_SENSORS;
  
  delay(500);  

  Serial.println();
  ElementsProtocol protocol(WiFi.macAddress(), deviceName, deviceDesc, deviceNoOfSensors, &mqtt);
  while(!protocol.connectedToServer)
  {
    protocol.connectToServer();
    Serial.println("Connected");
  }

}

void loop() 
{
  MQTT_connect(&mqtt);

}
