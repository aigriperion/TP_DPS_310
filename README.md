# TP DPS 310 - Projet

## Description
Ce projet implémente un programme pour lire les données d'un capteur DPS310, calculer la température et envoyer les données via MQTT.

## Arborescence du projet
```
.
├── include
│   ├── dps310.h
│   └── mqtt_client.h
├── Makefile
├── README.md
└── src
    ├── dps310.c
    ├── main.c
    └── mqtt_client.c
```

## Commandes Makefile

Le projet utilise un `Makefile` pour simplifier la compilation et l'exécution. Voici les commandes disponibles :

### Commandes principales

1. **`make` ou `make all`**  
   Compile l'exécutable principal du projet en utilisant tous les fichiers sources.  
   L'exécutable est généré dans le dossier `bin` sous le nom `dps310_reader`.

2. **`make run`**  
   Compile le projet si nécessaire et exécute l'exécutable principal `bin/dps310_reader`.

3. **`make clean`**  
   Supprime tous les fichiers générés, y compris l'exécutable dans le dossier `bin`.

## Exemple d'utilisation

1. **Compiler le projet** :
   ```bash
   make
   ```

2. **Exécuter le programme** :
   ```bash
   make run
   ```

3. **Nettoyer les fichiers générés** :
   ```bash
   make clean
   ```

## Dépendances
Compilateur GCC :
Assurez-vous que GCC est installé pour compiler le code C.
```bash
sudo apt-get install build-essential
```

Le projet utilise la bibliothèque MQTT Paho. Assurez-vous qu'elle est installée sur votre système avant de compiler le projet.  
Pour installer la bibliothèque sur une distribution basée sur Debian, utilisez la commande suivante :
```bash
sudo apt-get install libpaho-mqtt-dev
```