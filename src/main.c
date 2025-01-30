#include "battle_c.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Définition de la structure pour stocker les informations des objets
typedef struct {
    char type[50];
    float position_x;
    float position_y;
} ObjectInfo;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Fonction permettant d'afficher les données du joueur courant
void print_data_current_player(BC_Connection *connection){
  BC_PlayerData player = bc_get_player_data(connection);
    printf("ID : %d\n", player.id);
    printf("Vie du joueur : %d\n", player.health);
    printf("Armure du joueur : %d\n", player.armor);
    printf("Score : %d\n", player.score);
    printf("Position x: %.2f\n", player.position.x);
    printf("Position y: %.2f\n", player.position.y);
}

double calculate_shoot_angle(BC_Connection *connection, double enemy_x, double enemy_y) {
    // Récupère la position actuelle du joueur
    BC_PlayerData player = bc_get_player_data(connection);
    double player_x = player.position.x;
    double player_y = player.position.y;

    // Calcule l'angle en utilisant atan2 (donne l'angle en radians)
    double angle = atan2(enemy_y - player_y, enemy_x - player_x);

    printf("Angle calculé pour tirer sur l'ennemi : %.2f radians\n", angle);
    return angle;
}

 
// Fonction permettant de bouger le joueur
void move_player(BC_Connection *connection, double x, double y, double z){
  bc_set_speed(connection, x, y, z);
  printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
}

void shoot(BC_Connection *connection, double angle) {
    // Appelle la fonction bc_shoot pour effectuer le tir
    BC_ShootResult result = bc_shoot(connection, angle);

    // Affiche les résultats du tir
    printf("Tir effectué à un angle de %.2f radians\n", angle);
    printf("Succès : %s\n", result.success ? "Oui" : "Non");

    if (result.success) {
        printf("ID de la cible touchée : %d\n", result.target_id);
        printf("Points de dégâts infligés : %d\n", result.damage_points);
        printf("Cible détruite : %s\n", result.target_destroyed ? "Oui" : "Non");
    } else {
        printf("Aucune cible touchée.\n");
    }
}


// Définition de la structure pour stocker les informations des objets
typedef struct {
    char type[50];
    float position_x;
    float position_y;
} ObjectInfo;

// Fonction faisant office de radar, permet donc de piger les objets proches du joueur et d'afficher leurs informations
ObjectInfo* radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters, int *count) {
    
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
            strncpy(object_infos[index].type, ConvertObjectTypeToString(object->type), sizeof(object_infos[index].type) - 1);
            object_infos[index].type[sizeof(object_infos[index].type) - 1] = '\0';
            object_infos[index].position_x = object->position.x;
            object_infos[index].position_y = object->position.y;

            index++;
        }
        current = bc_ll_next(current);
    }

    bc_ll_free(list);

    // Retourner le nombre d'objets trouvés
    *count = object_count;
    printf("Nombre d'objets trouvés : %d\n", count);

    return object_infos;
}

// Fonction qui cherche dans la liste chainée du radar l'ennemi le plus proche et renvoie sa position 
int find_closest_enemy(ObjectInfo *objects, int count, float player_x, float player_y, double *enemy_x, double *enemy_y) {
    if (count == 0) {
        printf("Aucun ennemi détecté.\n");
        return 0; // Aucun ennemi trouvé
    }

    double min_distance = INFINITY;
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(objects[i].type, "PLAYER") == 0) { // Vérifie si l'objet est un joueur (ennemi)
            double distance = sqrt(pow(objects[i].position_x - player_x, 2) + pow(objects[i].position_y - player_y, 2));
            if (distance < min_distance) {
                min_distance = distance;
                *enemy_x = objects[i].position_x;
                *enemy_y = objects[i].position_y;
                found = 1;
            }
        }
    }

    if (found) {
        printf("Ennemi le plus proche trouvé à x: %.2f, y: %.2f (Distance: %.2f)\n", *enemy_x, *enemy_y, min_distance);
    } else {
        printf("Aucun ennemi détecté dans le rayon.\n");
    }

    return found;
}

int main(int argc, char *argv[]) {
    BC_Connection *conn = bc_connect("5.135.136.236", 8080);

    if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }
    printf("Connecté au serveur avec succès !\n");

    // Afficher les données du joueur
    print_data_current_player(conn);

    // Permet de bouger le joueur
    move_player(conn, 1, 1, 1);

    float player_x = bc_get_player_data(conn).position.x; 
    float player_y = bc_get_player_data(conn).position.y;
    // Position du joueur
    BC_PlayerData player = bc_get_player_data(conn);
    float player_x = player.position.x;
    float player_y = player.position.y;

    // Radar settings
    float detection_radius_meters = 10.0f;
    // Radar
    // while(true){
    radar(conn, player_x, player_y, detection_radius_meters, 0);
    // }
    float meters_to_pixels = 50.0f;
    int object_count = 0;

    // Scanner les objets autour
    ObjectInfo *objects = radar(conn, player_x, player_y, detection_radius_meters, meters_to_pixels, &object_count);

    if (objects != NULL) {
        double enemy_x, enemy_y;

        // Trouver l'ennemi le plus proche
        if (find_closest_enemy(objects, object_count, player_x, player_y, &enemy_x, &enemy_y)) {
            // Calculer l'angle de tir
            double angle = calculate_shoot_angle(conn, enemy_x, enemy_y);

            // Tirer sur l'ennemi
            shoot(conn, angle);
        }

        free(objects); // Libérer la mémoire
    }

    return EXIT_SUCCESS;
}
