/*
  ArduinoMqttClient - WiFi Simple Sender

  This example connects to a MQTT broker and publishes a message to
  a topic once a second.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board

  This example code is in the public domain.
*/

#include <ArduinoMqttClient.h>
#include <WiFi101.h>
//#include <ThingsBoard.h>
///////you may enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "BELL301";
char pass[] = "4DE59E5ADEE7";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
//ThingsBoard tb(wifiClient);
unsigned long lastSend;
const char broker[] = "thingsboard.cloud";
int        port     = 1883;
const char topic[]  = "v1/devices/me/telemetry";

String username = "yPrVC9HCSNZBxfvyxgKZ"; //authentication token here

String password = "";

const long interval = 1000;
unsigned long previousMillis = 0;

int buttonPin = 1;

#include "thingProperties.h"
#include <DHT.h>
#define DHTPIN 6

#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  //pin setup
  pinMode(buttonPin, INPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  dht.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");
  
  mqttClient.setUsernamePassword(username, password);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  lastSend = 0;
}

//String msgPayload = "{\"temperature\":25}";



void loop() {
int Temperature;

  ArduinoCloud.update();
  // Your code here 
    // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();
if(millis() - lastSend>1000) {
  Temperature=dht.readTemperature();
 // mqttClient.sendTelemetryFloat("temperature1", Temperature);
  lastSend=millis();
}
String msgPayload = "{\"temperature\":";
msgPayload += dht.readTemperature();
msgPayload += "}";

  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(topic);
  mqttClient.print(msgPayload);
  mqttClient.endMessage();
  

  // send message, the Print interface can be used to set the message contents
  Serial.print("Sending message to topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(msgPayload);
  Serial.println();

  delay(5000);
  
}

void onVChange() {
  // Do something
}
