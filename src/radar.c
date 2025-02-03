#include "radar.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>



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
            object_infos[index].type = ConvertObjectTypeToString(object->type);
            object_infos[index].position_x = object->position.x;
            object_infos[index].position_y = object->position.y;
            object_infos[index].id = object->id;

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

