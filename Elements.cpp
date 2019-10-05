#include "Arduino.h"
#include "Elements.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Arduino_JSON.h"
#include "ESP8266WiFi.h"

ElementsProtocol::ElementsProtocol(String deviceMAC, String deviceName, String deviceDesc, uint8_t deviceNoOfSensors, Adafruit_MQTT_Client *mqttserver)
{
  _deviceMAC = deviceMAC;
  _deviceName = deviceName;
  _deviceDesc = deviceDesc;
  _deviceNoOfSensors= deviceNoOfSensors;
  _mqtt = mqttserver;
  connectedToServer = false;
}

void ElementsProtocol::connectToServer()
{  
  JSONVar initHandshake;

  initHandshake["deviceMAC"] = _deviceMAC;
  initHandshake["deviceName"] = _deviceName;
  initHandshake["deviceDesc"] = _deviceDesc;
  initHandshake["deviceNoOfSensors"] = _deviceNoOfSensors;
  Serial.println("First Handshake JSON");
  Serial.println(JSON.stringify(initHandshake));

  const char response[] = "{\"status\":\"ok\"}";

  JSONVar jsonResponse = JSON.parse(response);

  if(strcmp((const char*)jsonResponse["status"], "ok") == 0)
  {
    connectedToServer = true;
  }
  else
  {
    connectedToServer = false;
  }
}
