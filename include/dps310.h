#ifndef DPS310_H
#define DPS310_H

#include <stdint.h>

// Registres connus
enum RegOffset_e {
    REG_TEMP = 0x03,
    REG_COEF1 = 0x10,
    REG_COEF2 = 0x11,
    REG_COEF3 = 0x12
};

// Facteurs de suréchantillonnage
enum OversamplingRate_e {
    OSR_SINGLE = 0x00
};

// Prototypes des fonctions
int32_t _signed24_to_signed32(int32_t value);
int32_t _get_s24_at(const uint8_t *regmap, int offset);
int32_t _get_temperature_raw(const uint8_t *regmap);
float _get_temperature_real(const uint8_t *regmap);

#endif // DPS310_H