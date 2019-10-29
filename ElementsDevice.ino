#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "Elements.h"

#include "DHT.h"

/**WIFI SETUP****************************/
#define WLAN_SSID "user"
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

ElementsProtocol protocol(WiFi.macAddress(), DEVICE_NAME, DEVICE_DESC, DEVICE_NO_OF_SENSORS, &mqtt);


/**TEMP/HUMIDITY SETUP************************/
#define DHTPIN D4
#define DHTTYPE DHT12

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  // Start Serial Communication
  Serial.begin(9600);
  while(!Serial);

  delay(500);  

  Serial.println();

  // Connect to the Elements Server.   
  while(!protocol.connectedToServer)
  {
    protocol.connectToServer();
  }
  
  Serial.println("Connected to Elements Server");

  // Begin communication to the Temp/Humidity Sensor
  dht.begin();
}

void loop() 
{
  // Perform Measurement every 5000ms
  delay(5000);

  // Ensure devices is connected to MQTT server
  MQTT_connect(&mqtt);  

  // Read data from the sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Basic Validity Checks
  if (isnan(h) || isnan(t)) {
    Serial.println("Broken Value, Returning!");
    return;
  }

  // Create Transmission Data Structure  
  float data[DEVICE_NO_OF_SENSORS];  

  data[0] = h;
  data[1] = t;

  // Send Data Structure via MQTT Packet using the Elements Protocol
  protocol.transmitData(data, DEVICE_NO_OF_SENSORS);
}
