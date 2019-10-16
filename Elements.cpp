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
  Adafruit_MQTT_Publish tx = Adafruit_MQTT_Publish(_mqtt, "new_devices");
  Adafruit_MQTT_Subscribe rx = Adafruit_MQTT_Subscribe(_mqtt, _deviceMAC.c_str());

  _mqtt->subscribe(&rx);

  MQTT_connect(_mqtt);
  
  JSONVar initHandshake;

  initHandshake["deviceMAC"] = _deviceMAC;
  initHandshake["deviceName"] = _deviceName;
  initHandshake["deviceDesc"] = _deviceDesc;
  initHandshake["deviceNoOfSensors"] = _deviceNoOfSensors;
  Serial.println("First Handshake JSON");  
  Serial.println(JSON.stringify(initHandshake));

  if(!tx.publish(JSON.stringify(initHandshake).c_str()))
  {
    Serial.println("Transmission Failed");
  }
  else
  {
    Serial.println("Transmission Success");
  }

  Adafruit_MQTT_Subscribe *subscription;

  while((subscription = _mqtt->readSubscription(10000)))
  {
    Serial.println("runs");
    
    if(subscription == &rx)
    {
      Serial.println("RECV:");
      char* response = (char*)rx.lastread;
      Serial.println(response);

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
