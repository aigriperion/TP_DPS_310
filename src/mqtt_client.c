#include "../include/mqtt_client.h"
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

void send_temperature_mqtt(float temperature) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }

    char payload[50];
    snprintf(payload, sizeof(payload), "Temperature: %.1f °C", temperature);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Publishing message: %s\n", payload);

    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message delivered\n");

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}