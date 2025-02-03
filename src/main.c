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
    float id;
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
    fflush(stdout);
}
 
// Fonction permettant au joueur de tirer
void shoot(BC_Connection *connection, double angle) {
    BC_ShootResult result = bc_shoot(connection, angle);
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

double calculate_shoot_angle(BC_Connection *connection, double enemy_x, double enemy_y) {
    BC_PlayerData player = bc_get_player_data(connection);
    double player_x = player.position.x;
    double player_y = player.position.y;
    double angle = atan2(enemy_y - player_y, enemy_x - player_x);
    printf("Angle calculé pour tirer sur l'ennemi : %.2f radians\n", angle);
    return angle;
}

void move_player(BC_Connection *connection, double x, double y, double z){
  bc_set_speed(connection, x, y, z);
  printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
}

ObjectInfo* radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters, int *count) {
    BC_List *list = bc_radar_ping(connection);
    BC_List *current = list;
    int object_count = 0;

    while (current != NULL) {
        BC_MapObject *object = bc_ll_value(current);
        float distance = sqrt(pow(object->position.x - player_x, 2) + pow(object->position.y - player_y, 2));
        if (distance <= detection_radius_meters) {
            object_count++;
        }
        current = bc_ll_next(current);
    }

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
            object_infos[index].type = ConvertObjectTypeToString(object->type);
            object_infos[index].position_x = object->position.x;
            object_infos[index].position_y = object->position.y;
            object_infos[index].id = object->id;
            index++;
            fflush(stdout);
        }
        current = bc_ll_next(current);
    }

    *count = object_count;
    printf("Nombre d'objets trouvés : %d\n", *count);
    fflush(stdout);
    bc_ll_free(list);
    return object_infos;
}

void detect_and_shoot_nearest_enemy(BC_Connection *connection, float player_x, float player_y, float detection_radius) {
    int count;
    BC_PlayerData player = bc_get_player_data(connection);
    int my_id = player.id;
    ObjectInfo *objects = radar(connection, player_x, player_y, detection_radius, &count);
    int nearest_id = -1;
    float nearest_distance = detection_radius;
    float nearest_x, nearest_y;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(objects[i].type, "PLAYER") == 0 && objects[i].id != my_id) {
            float distance = sqrt(pow(objects[i].position_x - player_x, 2) + pow(objects[i].position_y - player_y, 2));
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_x = objects[i].position_x;
                nearest_y = objects[i].position_y;
                nearest_id = objects[i].id;
            }
        }
    }
    
    if (nearest_id != -1) {
        double angle = calculate_shoot_angle(connection, nearest_x, nearest_y);
        printf("Angle: %.2f radians, ID ennemi: %d, Distance: %.2f\n", angle, nearest_id, nearest_distance);
        shoot(connection, angle);
    }
    else {
        printf("Aucun ennemi détecté\n");
    }
    free(objects);
}



int main(int argc, char *argv[]) {
    BC_Connection *conn = bc_connect("5.135.136.236", 8080);
    if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }
    printf("Connecté au serveur avec succès letsgo !\n");
    fflush(stdout);
    bc_get_world_info(conn);
    print_data_current_player(conn);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    move_player(conn, 1, 1, 1);
    float player_x = bc_get_player_data(conn).position.x; 
    float player_y = bc_get_player_data(conn).position.y;
    float detection_radius_meters = 10.0f;
    detect_and_shoot_nearest_enemy(conn, player_x, player_y, detection_radius_meters);
    printf("Fin du radar\n");
    fflush(stdout);
    return EXIT_SUCCESS;
}