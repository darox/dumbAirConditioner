#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

const char* ssid = "ssid";
const char* password =  "password";
const char* mqttServer = "fqdn";
const int mqttPort = 8883;
const char* mqttUser = "user";
const char* mqttPassword = "password";
String command;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.println("Command arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Command: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    command += ((char)payload[i]);
  }
  
  if (command == "0x12181120") {
      irsend.sendNEC(0x12181120, 32); // This turns on the Air Conditioner
     
  }
  else {
    irsend.sendNEC(0x12188020, 32); // This turns off the Air Conditioner
  }

  command = "";

}
  
 

void setup() {
 
  irsend.begin();
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.subscribe("topic");
}
 
void loop() {
  client.loop();
}
