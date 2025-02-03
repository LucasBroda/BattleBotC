/**
 * @file radar.c
 * @brief Implémentation du radar pour détecter les objets à proximité du joueur.
 *
 * Ce fichier contient les fonctions permettant de scanner l'environnement du joueur,
 * d'identifier les objets proches et d'afficher leurs informations.
 *
 */

#include "radar.h"   /**< Gestion du radar et détection d'objets */
#include "stdio.h"
#include "stdlib.h"
#include <math.h>




// Fonction permettant de convertir les types d'object de l'enum en chaine de caractère

/**
 * @brief Convertit un type d'objet (enum) en une chaîne de caractères.
 *
 * Cette fonction permet d'afficher un type d'objet sous forme lisible (ex: "PLAYER", "WALL").
 *
 * @param type Type de l'objet à convertir.
 * @return Une chaîne de caractères correspondant au type d'objet.
 */
char* ConvertObjectTypeToString(enum BC_ObjectType type) {
    switch (type) {
        case OT_PLAYER: return "PLAYER";
        case OT_WALL:   return "WALL";
        case OT_BOOST:  return "BOOST";
        default:        return "UNKNOWN";
    }
}

/**
 * @brief Détecte les objets proches du joueur en utilisant un radar.
 *
 * Cette fonction effectue un scan de l'environnement du joueur, récupère les objets
 * à proximité dans un rayon donné et affiche leurs informations. Elle retourne également
 * un tableau d'objets détectés.
 *
 * @param connection Pointeur vers la connexion au serveur de jeu.
 * @param player_x Position actuelle du joueur sur l'axe X.
 * @param player_y Position actuelle du joueur sur l'axe Y.
 * @param detection_radius_meters Rayon de détection en mètres.
 * @param count Pointeur pour stocker le nombre d'objets détectés.
 * @return Un tableau d'objets détectés (ObjectInfo*), à libérer après usage.
 */
ObjectInfo* radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters, int *count) {

    /** @brief Liste des objets détectés par le radar */
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
