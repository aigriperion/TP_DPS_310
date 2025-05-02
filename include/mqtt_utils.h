#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

#include <mosquitto.h>

#define HOST "localhost"
#define PORT 1883
#define KEEP_ALIVE 60
#define TOPIC "thprobe/test/temperature"

// Initialisation de la bibliothèque Mosquitto
int mqtt_lib_init();

// Création et retour d'une instance Mosquitto
struct mosquitto* mqtt_create_client();

// Connexion au broker MQTT
int mqtt_connect(struct mosquitto *mosq);

// Publication d'un message sur le topic
int mqtt_publish(struct mosquitto *mosq, const char *payload);

// Déconnexion et nettoyage de l'instance Mosquitto
void mqtt_disconnect_and_cleanup(struct mosquitto *mosq);

#endif
