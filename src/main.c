#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mqtt_utils.h"

int main() {
    struct mosquitto *mosq = NULL;
    int rc;

    // Initialiser la bibliothèque Mosquitto
    rc = mqtt_lib_init();
    if (rc != MOSQ_ERR_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Créer une nouvelle instance
    mosq = mqtt_create_client();
    if (!mosq) {
        return EXIT_FAILURE;
    }

    // Se connecter au broker MQTT
    rc = mqtt_connect(mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        mqtt_disconnect_and_cleanup(mosq);
        return EXIT_FAILURE;
    }

    // Boucle d'envoi de valeurs fictives
    for (int i = 0; i < 5; ++i) {
        int temp = 230 + i;  // Ex: 23.0°C, 23.1°C, etc.
        char payload[32];
        snprintf(payload, sizeof(payload), "%d", temp); // en dixièmes de °C

        rc = mqtt_publish(mosq, payload);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Erreur d'envoi : %s\n", mosquitto_strerror(rc));
        }

        sleep(1); // Pause 1 seconde
    }

    // Nettoyage
    mqtt_disconnect_and_cleanup(mosq);

    return EXIT_SUCCESS;
}
