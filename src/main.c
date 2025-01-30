#include "battle_c.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Définition de la structure pour stocker les informations des objets
typedef struct {
    char* type;
    float position_x;
    float position_y;
} ObjectInfo;

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


// Définition de la structure pour stocker les informations des objets
typedef struct {
    char type[50];
    float position_x;
    float position_y;
} ObjectInfo;

// Fonction faisant office de radar, permet donc de piger les objets proches du joueur et d'afficher leurs informations
ObjectInfo* radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters,int *count) {
    
    BC_List *list = bc_radar_ping(connection);
    BC_List *current = list;
    int object_count = 0;

    // Compter le nombre d'objets dans le rayon de détection
    while (current != NULL) {
        BC_MapObject *object = bc_ll_value(current);
        float distance = sqrt(pow(object->position.x - player_x, 2) + pow(object->position.y - player_y, 2));
        if (distance <= detection_radius_pixels) {
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
        float distance = sqrt(pow(object->position.x - player_x, object->position.y));
        if (distance <= detection_radius_pixels) {
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

int main(int argc, char *argv[])
{

  BC_Connection *conn = bc_connect("5.135.136.236", 8080);

  if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }
    printf("Connecté au serveur avec succès letsgo !\n");
    fflush(stdout);
    // Information sur le monde courant
    printf("Information sur le monde courant\n");
    fflush(stdout);
    bc_get_world_info(conn);

    // Affiche les données du joueur courant
    printf("Affichage des données du joueur courant\n");
    fflush(stdout);
    print_data_current_player(conn);

    // Permet de bouger le joueur
    move_player(conn, 1, 1, 1);

    float player_x = bc_get_player_data(conn).position.x; 
    float player_y = bc_get_player_data(conn).position.y;
    float detection_radius_meters = 10.0f;

    // Radar
    // while(true){
    int player_count = 0;
    radar(conn, player_x, player_y, detection_radius_meters, &player_count);
    // }
    printf("Fin du radar\n");
    fflush(stdout);



  return EXIT_SUCCESS;
}
