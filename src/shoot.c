#include "battle_c.h"
#include "radar.h"
#include "move.h"
#include "print.h"
#include "shoot.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <math.h>


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