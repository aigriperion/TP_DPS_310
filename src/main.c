#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include "../include/libdps310.h"
#include "mqtt_client.h"

volatile int interval = 5; // Intervalle par défaut en secondes

// Gestionnaire de commandes MQTT
void handle_command(const char *command) {
    if (strcmp(command, "read") == 0) {
        printf("Command received: read\n");
        float temperature = dps310_read_temperature();
        float pressure = dps310_read_pressure();
        printf("Immediate Read - Temperature: %.1f °C, Pressure: %.2f mbar\n", temperature, pressure);
    } else if (strncmp(command, "set_interval", 12) == 0) {
        char *value = (char *)(command + 13); // Extraire la valeur après "set_interval "
        if (!isdigit(*value)) {
            printf("Invalid interval value: %s\n", value);
            return;
        }
        int new_interval = atoi(value);
        if (new_interval == 0) {
            printf("Command received: set_interval 0 (pause)\n");
            interval = 0;
        } else if (new_interval > 0) {
            printf("Command received: set_interval %d\n", new_interval);
            interval = new_interval;
        }
    } else {
        printf("Unknown command: %s\n", command);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [NAME] [FREQ]\n", argv[0]);
        return 1;
    }

    char *name = argv[1];
    if (argc == 3) {
        interval = atoi(argv[2]);
        if (interval <= 0) {
            fprintf(stderr, "FREQ must be a positive integer.\n");
            return 1;
        }
    }

    dps310_init();

    // Préparer les topics pour les données et les commandes
    char cmd_topic[256];
    char topic_temp[256];
    char topic_pressure[256];
    snprintf(cmd_topic, sizeof(cmd_topic), "thprobe/%s/cmd", name);
    snprintf(topic_temp, sizeof(topic_temp), "thprobe/%s/data/temperature", name);
    snprintf(topic_pressure, sizeof(topic_pressure), "thprobe/%s/data/pressure", name);

    // Préparer les arguments pour le thread
    subscribe_args_t *args = malloc(sizeof(subscribe_args_t));
    if (!args) {
        fprintf(stderr, "Failed to allocate memory for thread arguments.\n");
        return 1;
    }
    strncpy(args->topic, cmd_topic, sizeof(args->topic));
    args->command_handler = handle_command;

    // Lancer un thread pour écouter les commandes MQTT
    pthread_t cmd_thread;
    pthread_create(&cmd_thread, NULL, subscribe_to_commands_thread, args);

    while (1) {
        if (interval > 0) {
            float temperature = dps310_read_temperature();
            float pressure = dps310_read_pressure();

            printf("Temperature: %.1f °C\n", temperature);
            printf("Pressure: %.2f mbar\n", pressure);

            // Publier les données sur les topics dédiés
            send_temperature_mqtt(topic_temp, temperature);
            send_pressure_mqtt(topic_pressure, pressure);

            sleep(interval);
        } else {
            sleep(1); // Pause active
        }
    }

    pthread_join(cmd_thread, NULL);
    dps310_shutdown();
    return 0;
}