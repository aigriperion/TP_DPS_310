#include <stdio.h>
#include <stdlib.h>  // Pour atoi
#include <unistd.h>  // Pour sleep
#include "dps310.h"
#include "mqtt_client.h"

// Registre simulé du DPS310
const uint8_t dps310_regmap[] = {
    0xf8, 0x01, 0x90, 0x02, 0x34, 0xf2, 0x26, 0xa0, 0xc7, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x0d, 0xee, 0xd6, 0x12, 0xe4, 0xbf, 0x43, 0xc0, 0xfb, 0xcb, 0x03, 0x7d, 0xe8, 0x9e, 0x00, 0x05,
    0xfd, 0xbd, 0x00, 0x00, 0x29, 0x53, 0x04, 0x70, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x17, 0x00, 0x66, 0xc3, 0x0a, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};

int main(int argc, char ** argv) {
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

    float temperature = _get_temperature_real(dps310_regmap);
    printf("\nMeasured temperature is %.1f °C\n", temperature);

    if (argc == 3) {
        char topic[256];
        snprintf(topic, sizeof(topic), "thprobe/%s/temperature", name);

        while (1) {
            send_temperature_mqtt(topic, temperature);
            printf("Temperature sent to MQTT topic: %s\n", topic);
            sleep(freq);
        }
    }

    return 0;
}