#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#define ADDRESS     "tcp://broker.hivemq.com:1883"
#define CLIENTID    "DPS310_Client"
#define TOPIC       "thprobe/TOURET&GIRARD/temperature"
#define QOS         1
#define TIMEOUT     10000L

void send_temperature_mqtt(float temperature);

#endif // MQTT_CLIENT_H