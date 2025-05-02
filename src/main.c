#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/dps310.h"
#include "../include/mqtt_utils.h"

#define DUMP_PATH "../dump/dps310_dump.txt"

// Fonction pour publier une mesure sur MQTT
int send_measurement_to_mqtt(struct mosquitto *mosq, const dps310_measurement *measurement) {
    // Création du message JSON
    char payload[256];
    snprintf(payload, sizeof(payload), "{\"temperature\": %.2f, \"coeff1\": %d, \"coeff2\": %d, \"coeff3\": %d}",
             measurement->temperature, measurement->coeff1, measurement->coeff2, measurement->coeff3);

    // Envoi du message MQTT
    return mqtt_publish(mosq, payload);
}

int main() {
    const char *filename = DUMP_PATH;
    dps310_measurement *measurements = NULL;
    int count = 0;

    // Initialiser la bibliothèque Mosquitto
    int rc = mqtt_lib_init();
    if (rc != MOSQ_ERR_SUCCESS) {
        return EXIT_FAILURE;
    }

    // Créer un client Mosquitto
    struct mosquitto *mosq = mqtt_create_client();
    if (!mosq) {
        return EXIT_FAILURE;
    }

    // Se connecter au broker MQTT
    rc = mqtt_connect(mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        mqtt_disconnect_and_cleanup(mosq);
        return EXIT_FAILURE;
    }

    // Lire les mesures depuis le fichier
    rc = read_dps310_dump(filename, &measurements, &count);
    if (rc != 0) {
        mqtt_disconnect_and_cleanup(mosq);
        return EXIT_FAILURE;
    }

    // Publier les mesures sur MQTT
    for (int i = 0; i < count; i++) {
        rc = send_measurement_to_mqtt(mosq, &measurements[i]);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Erreur lors de l'envoi de la mesure %d\n", i + 1);
        }
        sleep(1);  // Pause de 1 seconde entre chaque envoi
    }

    // Libérer la mémoire des mesures
    free_measurements(measurements);

    // Déconnexion et nettoyage
    mqtt_disconnect_and_cleanup(mosq);

    return EXIT_SUCCESS;
}
