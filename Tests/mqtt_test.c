#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <unistd.h> 

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define TOPIC "thprobe/test/temperature"

int main() {
    struct mosquitto *mosq = NULL;
    int rc;

    // Initialiser la bibliothèque Mosquitto
    mosquitto_lib_init();

    // Créer une nouvelle instance
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Erreur : Impossible de créer le client Mosquitto\n");
        return EXIT_FAILURE;
    }

    // Se connecter au broker MQTT local
    rc = mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Erreur de connexion : %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        return EXIT_FAILURE;
    }

    // Boucle d'envoi de valeurs fictives
    for (int i = 0; i < 5; ++i) {
        int temp = 230 + i;  // Ex: 23.0°C, 23.1°C, etc.
        char payload[32];
        snprintf(payload, sizeof(payload), "%d", temp); // en dixièmes de °C

        rc = mosquitto_publish(mosq, NULL, TOPIC, strlen(payload), payload, 0, false);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Erreur d'envoi : %s\n", mosquitto_strerror(rc));
        } else {
            printf("Message envoyé : %s\n", payload);
        }

        sleep(1); // Pause 1 seconde
    }

    // Nettoyage
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return EXIT_SUCCESS;
}
