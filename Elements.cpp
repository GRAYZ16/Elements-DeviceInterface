#define DEBUG 1

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
  
  _tx = new Adafruit_MQTT_Publish(_mqtt, _deviceMAC.c_str());
  _rx = new Adafruit_MQTT_Subscribe(_mqtt, _deviceMAC.c_str());
  _mqtt->subscribe(_rx);
    
  connectedToServer = false;
}

void ElementsProtocol::connectToServer()
{  
  Adafruit_MQTT_Publish initTx = Adafruit_MQTT_Publish(_mqtt, "new_devices");
  
  MQTT_connect(_mqtt);
  
  JSONVar initHandshake;

  initHandshake["deviceMAC"] = _deviceMAC;
  initHandshake["deviceName"] = _deviceName;
  initHandshake["deviceDesc"] = _deviceDesc;
  initHandshake["deviceNoOfSensors"] = _deviceNoOfSensors;

  if(Serial && DEBUG)
  {
    Serial.println("Initial Handshake JSON:");  
    Serial.println(JSON.stringify(initHandshake));
  }

  if(!initTx.publish(JSON.stringify(initHandshake).c_str()))
  {
    if(Serial && DEBUG) Serial.println("Initial Handshake Failed to Transmit");
  }
  else
  {
    if(Serial && DEBUG) Serial.println("Initial Handshake Transmission Success");
  }

  Adafruit_MQTT_Subscribe *subscription;

  while((subscription = _mqtt->readSubscription(10000)))
  {    
    if(subscription == _rx)
    {
      char* response = (char*)_rx->lastread;

      if(Serial && DEBUG)
      {
        Serial.println("Server Response:");
        Serial.println(response);
      }

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
  }
}

void ElementsProtocol::transmitData(float *data, int len)
{
  MQTT_connect(_mqtt);
  JSONVar dataArray;

  for(int i=0; i < len; i++)
  {
    dataArray[i] = data[i];
  }

  JSONVar txFrame;
  txFrame["deviceMAC"] = _deviceMAC;
  txFrame["data"] = dataArray;
  
  if(!_tx->publish(JSON.stringify(txFrame).c_str()))
  {
    if(Serial && DEBUG) Serial.println("Transmission Failed");
  }
  else
  {
    if(Serial && DEBUG) Serial.println("Transmission Success");
  }
}

// MQTT connection function writen by Adafruit for creating MQTT connections
void MQTT_connect(Adafruit_MQTT_Client *mqtt) 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt->connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt->connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt->connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt->disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
