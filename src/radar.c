#include "radar.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <linux/time.h>


// Fonction permettant de convertir les types d'object de l'enum en chaine de caractère
char* ConvertObjectTypeToString(enum BC_ObjectType type) {
    switch (type) {
        case OT_PLAYER: return "PLAYER";
        case OT_WALL:   return "WALL";
        case OT_BOOST:  return "BOOST";
        default:        return "UNKNOWN";
    }
}

// Fonction faisant office de radar, permet donc de piger les objets proches du joueur et d'afficher leurs informations
ObjectInfo* radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters,int *count) {
    static struct timespec last_ping_time = {0, 0};
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);

    // Calculer le temps écoulé depuis le dernier ping en millisecondes
    long elapsed_time_ms = (current_time.tv_sec - last_ping_time.tv_sec) * 1000 + 
                           (current_time.tv_nsec - last_ping_time.tv_nsec) / 1000000;

    // Si le dernier ping a été effectué il y a moins de 50 ms, attendre
    if (elapsed_time_ms < 50) {
        usleep((50 - elapsed_time_ms) * 1000); // Attendre le temps restant en microsecondes
        clock_gettime(CLOCK_MONOTONIC, &current_time); // Mettre à jour le temps actuel après l'attente
    }

    // Mettre à jour le temps du dernier ping
    last_ping_time = current_time;

    BC_List *list = bc_radar_ping(connection);
    BC_List *current = list;
    int object_count = 0;

    // Compter le nombre d'objets dans le rayon de détection
    while (current != NULL) {
        BC_MapObject *object = bc_ll_value(current);
        float distance = sqrt(pow(object->position.x - player_x, 2) + pow(object->position.y - player_y, 2));
        if (distance <= detection_radius_meters) {
            object_count++;
        }
        current = bc_ll_next(current);
    }

    // Allouer de la mémoire pour stocker les informations des objets
    ObjectInfo *object_infos = (ObjectInfo*)malloc(object_count * sizeof(ObjectInfo));
    if (object_infos == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    current = list;
    int index = 0;
    while (current != NULL) {
        BC_MapObject *object = bc_ll_value(current);
        float distance = sqrt(pow(object->position.x - player_x, 2) + pow(object->position.y - player_y, 2));
        if (distance <= detection_radius_meters) {
            printf("----------------------Nouveau scan----------------------\n");
            printf("ID : %d\n", object->id);
            printf("Type : %s\n", ConvertObjectTypeToString(object->type));
            printf("Vie : %d\n", object->health);
            printf("Position x: %.2f\n", object->position.x);
            printf("Position y: %.2f\n", object->position.y);

            // Stocker les informations dans la structure
            object_infos[index].type = ConvertObjectTypeToString(object->type);
            object_infos[index].position_x = object->position.x;
            object_infos[index].position_y = object->position.y;

            index++;
            fflush(stdout);
        }
        current = bc_ll_next(current);
    }

    // Retourner le nombre d'objets trouvés
    *count = object_count;
    printf("Nombre d'objets trouvés : %d\n", *count);
    fflush(stdout);

    bc_ll_free(list);

    return object_infos;
}

