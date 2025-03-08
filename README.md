# Implémentation du Modèle Maître-Esclave avec MPI

Ce projet implémente un modèle de distribution de données selon l'architecture maître-esclave en utilisant MPI (Message Passing Interface). Le programme répartit des données entre plusieurs processus esclaves, qui effectuent des calculs en parallèle et retournent les résultats au processus maître.

## Fonctionnalités
- Distribution de données depuis un processus maître vers plusieurs processus esclaves
- Traitement des données par les esclaves (mise au carré des valeurs reçues)
- Transmission des résultats au processus maître
- Gestion dynamique des demandes de données et des ordres de fin

## Prérequis
- **Compilateur C** avec support MPI (ex: `mpicc` fourni par OpenMPI)
- **OpenMPI** installé sur votre machine

## Compilation
Utilisez la commande suivante pour compiler le programme :
```bash
mpicc -o master_slave master_slave.c
```

## Exécution
Lancez le programme avec au moins 2 processus :
```bash
mpirun -np 4 ./master_slave
```
Où `-np 4` indique que le programme s'exécute avec 4 processus (1 maître et 3 esclaves).

## Explication du Code
- **Processus Maître :**
  1. Génère un tableau de données et détermine aléatoirement quel processus est le maître.
  2. Envoie des portions de données aux processus esclaves.
  3. Récupère les résultats des esclaves et leur envoie de nouvelles portions tant que des données restent à traiter.
  4. Envoie un signal de fin lorsque toutes les données ont été traitées.
  5. Affiche les données traitées.

- **Processus Esclave :**
  1. Demande des données au maître.
  2. Reçoit et traite les données (mise au carré des valeurs).
  3. Réexpédie les résultats au maître.
  4. Continue tant qu'il reçoit des données, s'arrête en recevant l'ordre de fin.

## Extrait de Code
```c
void traiter_donnees(int *donnees, int taille) {
    for (int i = 0; i < taille; i++) {
        donnees[i] = donnees[i] * donnees[i]; // Mise au carré des éléments
    }
}
```


---
Projet réalisé dans le cadre du TP HPC 2024-2025 à l'ESI.

