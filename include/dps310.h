#ifndef DPS310_H
#define DPS310_H

// Structure pour stocker une mesure DPS310
typedef struct {
    float temperature;  // Température en °C
    int coeff1;         // Coefficient 1
    int coeff2;         // Coefficient 2
    int coeff3;         // Coefficient 3
} dps310_measurement;

// Fonction pour lire les mesures depuis un fichier dump
int read_dps310_dump(const char *filename, dps310_measurement **measurements, int *count);

// Fonction pour libérer la mémoire allouée pour les mesures
void free_measurements(dps310_measurement *measurements);

#endif // DPS310_H
