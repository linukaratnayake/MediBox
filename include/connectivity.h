#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

extern WiFiClient espClient;
extern PubSubClient mqttClient;

void setupMQTT();
void connectToBroker();
void recieveCallback(char *, byte *, unsigned int);
void mqtt_publish(char *, float);

#endif