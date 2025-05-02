# TP DPS 310 - Projet

## Commandes Makefile

Ce projet utilise un `Makefile` pour faciliter la compilation et l'exécution des tests. Voici la liste des commandes disponibles dans le `Makefile` :

### Commandes principales

1. **`make all`**  
   Compile l'exécutable principal du projet en utilisant tous les fichiers sources.  
   Les fichiers objets sont générés dans le dossier `build`.

2. **`make run`**  
   Compile le projet si nécessaire et exécute l'exécutable principal `build/main`.

3. **`make clean`**  
   Supprime tous les fichiers générés, y compris les fichiers objets et l'exécutable dans le dossier `build`.

### Commandes de test

Les commandes suivantes permettent de compiler et exécuter des fichiers de test spécifiques situés dans le dossier `Tests`.

4. **`make test-%`**  
   Compile et exécute un fichier de test spécifique dans le dossier `Tests`.  
   Par exemple, pour compiler et exécuter le fichier `mqtt_test.c`, utilisez :  
   ```bash
   make test-mqtt_test
