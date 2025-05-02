#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dps310.h"

// Fonction pour lire les mesures depuis un fichier dump
int read_dps310_dump(const char *filename, dps310_measurement **measurements, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return -1;  // Erreur d'ouverture du fichier
    }

    // Calcul du nombre de lignes dans le fichier
    int num_lines = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        num_lines++;
    }

    // Réinitialisation du fichier à la première ligne
    fseek(file, 0, SEEK_SET);

    // Allocation de mémoire pour stocker les mesures
    *measurements = (dps310_measurement *)malloc(sizeof(dps310_measurement) * num_lines);
    if (!*measurements) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour les mesures\n");
        fclose(file);
        return -2;  // Erreur d'allocation mémoire
    }

    // Lecture des données du fichier
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        // Lire une ligne, parser les valeurs
        int coeff1, coeff2, coeff3;
        float temperature;

        // Utilisation de sscanf pour parser la ligne
        if (sscanf(line, "%f %d %d %d", &temperature, &coeff1, &coeff2, &coeff3) != 4) {
            fprintf(stderr, "Erreur : Format de ligne invalide dans le fichier %s\n", filename);
            free(*measurements);
            fclose(file);
            return -3;  // Erreur de format de ligne
        }

        // Stockage de la mesure dans le tableau
        (*measurements)[i].temperature = temperature;
        (*measurements)[i].coeff1 = coeff1;
        (*measurements)[i].coeff2 = coeff2;
        (*measurements)[i].coeff3 = coeff3;
        i++;
    }

    // Mise à jour du nombre de mesures
    *count = num_lines;

    fclose(file);
    return 0;  // Succès
}

// Fonction pour libérer la mémoire allouée pour les mesures
void free_measurements(dps310_measurement *measurements) {
    if (measurements) {
        free(measurements);
    }
}
