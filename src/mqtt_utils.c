#include "mqtt_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

int mqtt_lib_init() {
    int rc = mosquitto_lib_init();
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Erreur d'initialisation de la bibliothèque Mosquitto : %s\n", mosquitto_strerror(rc));
        return rc;
    }
    return MOSQ_ERR_SUCCESS;
}

struct mosquitto* mqtt_create_client() {
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Erreur : Impossible de créer le client Mosquitto\n");
        return NULL;
    }
    return mosq;
}

int mqtt_connect(struct mosquitto *mosq) {
    int rc = mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Erreur de connexion : %s\n", mosquitto_strerror(rc));
    }
    return rc;
}

int mqtt_publish(struct mosquitto *mosq, const char *payload) {
    int rc = mosquitto_publish(mosq, NULL, TOPIC, strlen(payload), payload, 0, false);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Erreur d'envoi : %s\n", mosquitto_strerror(rc));
    } else {
        printf("Message envoyé : %s\n", payload);
    }
    return rc;
}

void mqtt_disconnect_and_cleanup(struct mosquitto *mosq) {
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
