#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define CHUNKSIZE 10 // Taille de chaque portion de données traitée par les esclaves
#define TAG_DATA 1 // Étiquette pour les données
#define TAG_FINISH 2 // Étiquette pour l'ordre de fin

void traiter_donnees(int *donnees, int taille) {
    for (int i = 0; i < taille; i++) {
        donnees[i] = donnees[i] * donnees[i]; // Mise au carré des éléments
    }
}

int main(int argc, char **argv) {
    int rank, size; // Rang du processus, Nombre total de processus MPI
    int *data = NULL; // Tableau de données à distribuer
    int *buffer = NULL; // Tampon pour les processus esclaves
    int n = 100; // Taille totale du tableau de données
    int chunksize = CHUNKSIZE; // Taille de chaque portion de données
    int master; // Rang du maître
    int num_chunks; // Nombre total de portions de données à distribuer

    // Initialisation de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Récupérer le rang du processus
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Récupérer le nombre total de processus

    // Vérification que le nombre de processus est >= 2
    if (size < 2) {
        if (rank == 0) {
            printf("Erreur : le nombre de processus doit être supérieur ou égal à 2.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Déterminer aléatoirement le rang du maître
    master = rand() % size;

    // Calcul du nombre total de portions de données
    num_chunks = n / chunksize;

    if (rank == master) {
        // Processus maître
        data = (int *)malloc(n * sizeof(int));

        // Initialisation du tableau de données
        for (int i = 0; i < n; i++) {
            data[i] = i + 1; // Exemple : 1, 2, 3, ..., n
        }

        int chunks_sent = 0; // Nombre de portions envoyées
        int chunks_received = 0; // Nombre de portions reçues

        // Distribuer les données initiales aux esclaves
        for (int i = 0; i < size; i++) {
            if (i != master && chunks_sent < num_chunks) {
                MPI_Send(&data[chunks_sent * chunksize], chunksize, MPI_INT, i, TAG_DATA, MPI_COMM_WORLD);
                chunks_sent++;
            }
        }

        // Réception des résultats et distribution des portions restantes
        while (chunks_received < num_chunks) {
            MPI_Status status;
            int source;

            // Recevoir une portion traitée d'un esclave
            MPI_Recv(&data[chunks_received * chunksize], chunksize, MPI_INT, MPI_ANY_SOURCE, TAG_DATA, MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            chunks_received++;

            // Si des portions restent à envoyer, les distribuer
            if (chunks_sent < num_chunks) {
                MPI_Send(&data[chunks_sent * chunksize], chunksize, MPI_INT, source, TAG_DATA, MPI_COMM_WORLD);
                chunks_sent++;
            } else {
                // Sinon, envoyer un ordre de fin
                MPI_Send(NULL, 0, MPI_INT, source, TAG_FINISH, MPI_COMM_WORLD);
            }
        }

        // Envoyer l'ordre de fin aux esclaves restants
        for (int i = 0; i < size; i++) {
            if (i != master) {
                MPI_Send(NULL, 0, MPI_INT, i, TAG_FINISH, MPI_COMM_WORLD);
            }
        }

        // Affichage des données traitées
        printf("Données traitées par le maître : ");
        for (int i = 0; i < n; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        // Libération de la mémoire
        free(data);

    } else {
        // Processus esclave
        buffer = (int *)malloc(chunksize * sizeof(int));

        while (1) {
            MPI_Status status;

            // Réception d'une portion de données ou d'un ordre de fin
            MPI_Recv(buffer, chunksize, MPI_INT, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            // Vérifier si c'est un ordre de fin
            if (status.MPI_TAG == TAG_FINISH) {
                break;
            }

            // Traitement des données
            traiter_donnees(buffer, chunksize);

            // Envoyer les données traitées au maître
            MPI_Send(buffer, chunksize, MPI_INT, master, TAG_DATA, MPI_COMM_WORLD);
        }

        // Libération de la mémoire
        free(buffer);
    }

    // Finalisation de MPI
    MPI_Finalize();

    return 0;
}
