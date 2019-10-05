#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include "Elements.h"

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
  ElementsProtocol protocol(WiFi.macAddress(), deviceName, deviceDesc, deviceNoOfSensors, null);
  while(!protocol.connectedToServer)
  {
    protocol.connectToServer();
    Serial.println("Connected");
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
