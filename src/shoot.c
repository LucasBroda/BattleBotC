/**
 * @file shooting.c
 * @brief Implémentation des fonctions de tir et de détection des ennemis.
 */

#include "battle_c.h"
#include "radar.h"
#include "move.h"
#include "print.h"
#include "shoot.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <math.h>

/**
 * @brief Permet au joueur de tirer sur un ennemi.
 *
 * @param connection Pointeur vers la connexion du joueur.
 * @param angle Angle de tir en radians.
 */
void shoot(BC_Connection *connection, double angle) {
    BC_ShootResult result = bc_shoot(connection, angle);
    printf("Succes : %s\n", result.success ? "Oui" : "Non");
    if (result.success) {
        printf("Joueur touché : %d\n", result.target_id);
        printf("Dégâts infligés : %d\n", result.damage_points);
        printf("Joueur mort : %s\n", result.target_destroyed ? "Oui" : "Non");
    } else {
        printf("Personne n'a été touché \n");
    }
}

/**
 * @brief Calcule l'angle nécessaire pour tirer sur un ennemi donné.
 *
 * @param connection Pointeur vers la connexion du joueur.
 * @param enemy_x Position X de l'ennemi.
 * @param enemy_y Position Y de l'ennemi.
 * @return L'angle de tir en radians.
 */
double calculate_shoot_angle(BC_Connection *connection, double enemy_x, double enemy_y) {
    BC_PlayerData player = bc_get_player_data(connection);
    double player_position_x = player.position.x;
    double player_position_y = player.position.y;
    double angle = atan2(enemy_y - player_position_y, enemy_x - player_position_x);
    return angle;
}

/**
 * @brief Détecte et tire sur l'ennemi le plus proche dans un rayon donné.
 *
 * @param connection Pointeur vers la connexion du joueur.
 * @param player_position_x Position X du joueur.
 * @param player_position_y Position Y du joueur.
 * @param detection_radius Rayon de détection des ennemis.
 */
void detect_and_shoot_nearest_enemy(BC_Connection *connection, float player_position_x, float player_position_y, float detection_radius) {
    int count;
    BC_PlayerData player = bc_get_player_data(connection);
    int my_id = player.id;
    ObjectInfo *objects = radar(connection, player_position_x, player_position_y, detection_radius, &count);
    int nearest_id = -1;
    float nearest_distance = detection_radius;
    float nearest_x, nearest_y;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(objects[i].type, "PLAYER") == 0 && objects[i].id != my_id) {
            float distance = sqrt(pow(objects[i].position_x - player_position_x, 2) + pow(objects[i].position_y - player_position_y, 2));
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
        printf("Je décide de tirer sur: %d, qui est à une distance de: %.2f\n", nearest_id, nearest_distance);
        shoot(connection, angle);
    }
    else {
        printf("Pas d'ennemi en vue\n");
    }
    free(objects);
}
