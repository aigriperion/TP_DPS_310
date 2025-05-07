#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#define ADDRESS     "tcp://test.mosquitto.org:1883"
#define CLIENTID    "DPS310_Client"
#define QOS         1
#define TIMEOUT     10000L

void send_temperature_mqtt(const char *topic, float temperature);

#endif // MQTT_CLIENT_H