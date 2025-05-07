#include "../include/libdps310.h"
#include "../include/fakedps310.h"
#include <stdint.h>
#include <stdio.h>

// Table des facteurs d'échelle
static const uint32_t _scaleFactors[] = {
    524288,
    1572864,
    3670016,
    7864320,
    253952,
    516096,
    1040384,
    2088960
};

// Convertit un entier signé 24 bits en 32 bits
static int32_t _signed24_to_signed32(int32_t value) {
    if (value & 0x800000) {
        value |= 0xFF000000;
    }
    return value;
}

// Récupère une valeur 24 bits signée à un offset donné dans le registre
static int32_t _get_s24_at(const uint8_t *regmap, int offset) {
    int32_t value = (regmap[offset] << 16) | (regmap[offset + 1] << 8) | regmap[offset + 2];
    return _signed24_to_signed32(value);
}

// Récupère la température brute
static int32_t _get_temperature_raw(const uint8_t *regmap) {
    return _get_s24_at(regmap, REG_TEMP);
}

// Calcule la température réelle
static float _get_temperature_real(const uint8_t *regmap) {
    int32_t C0 = ((regmap[REG_COEF1] << 4) | (regmap[REG_COEF2] >> 4)) & 0xFFF;
    if (C0 & 0x800) {
        C0 |= 0xFFFFF000;
    }

    int32_t C1 = (((regmap[REG_COEF2] & 0x0F) << 8) | regmap[REG_COEF3]) & 0xFFF;
    if (C1 & 0x800) {
        C1 |= 0xFFFFF000;
    }

    int32_t Traw = _get_temperature_raw(regmap);
    float Traw_sc = Traw / (float)_scaleFactors[OSR_SINGLE];
    return C0 * 0.5f + C1 * Traw_sc;
}

// Initialise le capteur DPS310
void dps310_init() {
    fakedps310_power_on();
}

// Éteint le capteur DPS310
void dps310_shutdown() {
    fakedps310_power_off();
}

// Lit la température en degrés Celsius
float dps310_read_temperature() {
    const uint8_t *regmap = fakedps310_get_regmap();
    return _get_temperature_real(regmap);
}

// Lit la pression brute (ajoutez une fonction similaire si nécessaire)
static int32_t _get_pressure_raw(const uint8_t *regmap) {
    return _get_s24_at(regmap, REG_PRESS);
}

// Calcule la pression réelle (ajoutez une fonction similaire si nécessaire)
static float _get_pressure_real(const uint8_t *regmap) {
    int32_t C00 = (regmap[REG_COEF4] << 12) | (regmap[REG_COEF5] << 4) | (regmap[REG_COEF6] >> 4);
    if (C00 & 0x80000) {
        C00 |= 0xFFF00000;
    }

    int32_t C10 = ((regmap[REG_COEF6] & 0x0F) << 16) | (regmap[REG_COEF7] << 8) | regmap[REG_COEF8];
    if (C10 & 0x80000) {
        C10 |= 0xFFF00000;
    }

    int32_t Praw = _get_pressure_raw(regmap);
    float Praw_sc = Praw / (float)_scaleFactors[OSR_SINGLE];
    return C00 + C10 * Praw_sc;
}

// Lit la pression en millibars
float dps310_read_pressure() {
    const uint8_t *regmap = fakedps310_get_regmap();
    return _get_pressure_real(regmap);
}