#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "Arduino.h"
#include "Arduino_JSON.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

class ElementsProtocol
{
  public:
    ElementsProtocol(String MACAddr, Adafruit_MQTT_Client *mqttserver);
    void connectToServer();
    void transmitData(float *data, int len); 
    
  private:
    Adafruit_MQTT_Client* _mqtt;
    bool _connectedToServer;
    String _MACAddr;    
  
};




#endif //ELEMENTS_H
