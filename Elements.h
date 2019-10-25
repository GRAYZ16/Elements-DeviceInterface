#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "Arduino.h"
#include "Arduino_JSON.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Arduino_JSON.h"
#include "ESP8266WiFi.h"

class ElementsProtocol
{
  public:
    ElementsProtocol(String deviceMAC, String deviceName, String deviceDesc, uint8_t deviceNoOfSensors, Adafruit_MQTT_Client *mqttserver);
    void connectToServer();
    void transmitData(float *data, int len); 
    bool connectedToServer;
    
  private:
    Adafruit_MQTT_Client* _mqtt;
    String _deviceMAC;    
    String _deviceName;
    String _deviceDesc;
    uint8_t _deviceNoOfSensors;
    Adafruit_MQTT_Publish* _tx;
    Adafruit_MQTT_Subscribe* _rx;
};

void MQTT_connect(Adafruit_MQTT_Client *mqtt);



#endif //ELEMENTS_H
