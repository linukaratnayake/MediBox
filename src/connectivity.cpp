#include "connectivity.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setupMQTT()
{
    mqttClient.setServer("test.mosquitto.org", 1883);
    mqttClient.setCallback(recieveCallback);
}

void connectToBroker()
{
    while (!mqttClient.connected())
    {
        Serial.println("Attempting MQTT connection...");
        if (mqttClient.connect("ESP32Client-68435039325930567"))
        {
            Serial.println("MQTT Connected");
            mqttClient.subscribe("ENTC-ON-OFF_NI");
            mqttClient.subscribe("ENTC-ADMIN-SCH-ON_NI");
            mqttClient.subscribe("SERVO-ADJUSTMENTS_MINA");
            mqttClient.subscribe("SERVO-ADJUSTMENTS_CF");
        }
        else
        {
            Serial.print("Failed To connect to MQTT Broker");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}

void recieveCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    char payloadCharAr[length];
    Serial.print("Message Received: ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        payloadCharAr[i] = (char)payload[i];
    }
    Serial.println();

    if (strcmp(topic, "ENTC-ON-OFF_NI") == 0)
    {
        
    }
    else if (strcmp(topic, "ENTC-ADMIN-SCH-ON_NI") == 0)
    {

    }
    else if (strcmp(topic, "SERVO-ADJUSTMENTS_MINA") == 0)
    {

    }
    else if (strcmp(topic, "SERVO-ADJUSTMENTS_CF") == 0)
    {

    }
}

void mqtt_publish(char *topic, float value) {
  if ((int) value == value) {
    const int length = String(int(value)).length();
    char valueArr[length + 1];
    String(int(value)).toCharArray(valueArr, length + 1);
    mqttClient.publish(topic, valueArr);
  } else {
    const int length = String(value).length();
    char valueArr[length + 1];
    String(value).toCharArray(valueArr, length + 1);
    mqttClient.publish(topic, valueArr);
  }
}