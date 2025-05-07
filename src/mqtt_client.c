#include "../include/mqtt_client.h"
#include <stdio.h>
#include <string.h>
#include <MQTTClient.h>

// Fonction pour envoyer la température via MQTT
void send_temperature_mqtt(const char *topic, float temperature) {
    // Vérification de la validité de la température
    if (temperature < -40.0 || temperature > 85.0) { // Plage typique pour un capteur comme le DPS310
        printf("Invalid temperature value: %.2f °C. Skipping MQTT publish.\n", temperature);
        return;
    }

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    // Création du client MQTT
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connexion au broker MQTT
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }

    // Préparation du message à envoyer
    char payload[100];
    snprintf(payload, sizeof(payload), "Sensor: %s, Temperature: %.2f °C", CLIENTID, temperature);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    // Publication du message
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    printf("Publishing message to topic '%s': %s\n", topic, payload);

    // Attente de la confirmation de livraison
    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message delivered\n");

    // Déconnexion et destruction du client MQTT
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

// Fonction pour envoyer la pression via MQTT
void send_pressure_mqtt(const char *topic, float pressure) {
    if (pressure < 300.0 || pressure > 1100.0) { // Plage typique pour la pression atmosphérique en mbar
        printf("Invalid pressure value: %.2f mbar. Skipping MQTT publish.\n", pressure);
        return;
    }

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

    char payload[100];
    snprintf(payload, sizeof(payload), "Sensor: %s, Pressure: %.2f mbar", CLIENTID, pressure);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    printf("Publishing message to topic '%s': %s\n", topic, payload);

    MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message delivered\n");

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}