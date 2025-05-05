#include "../include/dps310.h"
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

int32_t _signed24_to_signed32(int32_t value) {
    if (value & 0x800000) {
        value |= 0xFF000000;
    }
    return value;
}

int32_t _get_s24_at(const uint8_t *regmap, int offset) {
    int32_t value = (regmap[offset] << 16) | (regmap[offset + 1] << 8) | regmap[offset + 2];
    return _signed24_to_signed32(value);
}

int32_t _get_temperature_raw(const uint8_t *regmap) {
    return _get_s24_at(regmap, REG_TEMP);
}

float _get_temperature_real(const uint8_t *regmap) {
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