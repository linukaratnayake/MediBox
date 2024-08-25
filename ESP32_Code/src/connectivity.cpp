#include "connectivity.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

int subscribedAngleOffset;
bool subscribedAngleOffsetChanged = false;
float subscribedGammaFactor;
bool subscribedGammaFactorChanged = false;

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
            mqttClient.subscribe("2853_MEDIBOX_ANGLE");
            mqttClient.subscribe("2853_MEDIBOX_FACTOR");
            mqttClient.subscribe("2853_MEDIBOX_ALARM");
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
    char payloadCharArr[length];
    Serial.print("Message Received: ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        payloadCharArr[i] = (char)payload[i];
    }
    Serial.println();

    if (strcmp(topic, "2853_MEDIBOX_ANGLE") == 0)
    {
        subscribedAngleOffset = atoi(payloadCharArr);
        subscribedAngleOffsetChanged = true;
    }
    else if (strcmp(topic, "2853_MEDIBOX_FACTOR") == 0)
    {
        subscribedGammaFactor = atoi(payloadCharArr);
        subscribedGammaFactorChanged = true;
    }
    else if (strcmp(topic, "2853_MEDIBOX_ALARM") == 0)
    {
        // Received data should be handled appropriately.
        // Detect which alarm, and assign it.
    }
}

void mqtt_publish(char *topic, float value)
{
    if ((int)value == value)
    {
        const int length = String(int(value)).length();
        char valueArr[length + 1];
        String(int(value)).toCharArray(valueArr, length + 1);
        mqttClient.publish(topic, valueArr);
    }
    else
    {
        const int length = String(value).length();
        char valueArr[length + 1];
        String(value).toCharArray(valueArr, length + 1);
        mqttClient.publish(topic, valueArr);
    }
}