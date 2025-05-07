#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/libdps310.h"
#include "mqtt_client.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [NAME] [FREQ]\n", argv[0]);
        return 1;
    }

    char *name = argv[1];
    int freq = 0;

    if (argc == 3) {
        freq = atoi(argv[2]);
        if (freq <= 0) {
            fprintf(stderr, "FREQ must be a positive integer.\n");
            return 1;
        }
    }

    dps310_init();

    while (1) {
        float temperature = dps310_read_temperature();
        float pressure = dps310_read_pressure();

        printf("Temperature: %.1f °C\n", temperature);
        printf("Pressure: %.2f mbar\n", pressure);

        char topic_temp[256];
        char topic_pressure[256];
        snprintf(topic_temp, sizeof(topic_temp), "thprobe/%s/temperature", name);
        snprintf(topic_pressure, sizeof(topic_pressure), "thprobe/%s/pressure", name);

        send_temperature_mqtt(topic_temp, temperature);
        send_temperature_mqtt(topic_pressure, pressure);

        sleep(freq);
    }

    dps310_shutdown();
    return 0;
}