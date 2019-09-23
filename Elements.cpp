#include "Arduino.h"
#include "Elements.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

ElementsProtocol::ElementsProtocol(String MACAddr, Adafruit_MQTT_Client *mqttserver)
{
  _MACAddr = MACAddr;
  _mqtt = mqttserver;
}
