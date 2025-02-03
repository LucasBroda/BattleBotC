/**
 * @file shoot.c
 * @brief Gestion des tirs dans le jeu Battle-C.
 * 
 * Ce fichier contient les fonctions permettant au joueur de tirer sur les ennemis,
 * de calculer l'angle de tir et de détecter l'ennemi le plus proche avant de tirer.
 *
 */

#include "battle_c.h" /**< Connexion au serveur et gestion du jeu */
#include "radar.h"    /**< Détection des objets environnants */
#include "move.h"     /**< Déplacement du joueur */
#include "print.h"    /**< Affichage des informations du joueur */
#include "shoot.h"    /**< Gestion des tirs */
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <math.h>

/**
 * @brief Permet au joueur de tirer dans une direction spécifique.
 * 
 * Cette fonction effectue un tir à un angle donné et affiche le résultat, y compris
 * si le tir a réussi, les dégâts infligés et si la cible a été détruite.
 * 
 * @param connection Pointeur vers la connexion au serveur de jeu.
 * @param angle Angle du tir en radians.
 */
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

/**
 * @brief Calcule l'angle de tir pour toucher un ennemi donné.
 * 
 * Cette fonction utilise les positions du joueur et de l'ennemi pour déterminer
 * l'angle optimal du tir en utilisant la fonction `atan2()`.
 * 
 * @param connection Pointeur vers la connexion au serveur de jeu.
 * @param enemy_x Position X de l'ennemi.
 * @param enemy_y Position Y de l'ennemi.
 * @return L'angle de tir en radians.
 */
double calculate_shoot_angle(BC_Connection *connection, double enemy_x, double enemy_y) {
    BC_PlayerData player = bc_get_player_data(connection);
    double player_x = player.position.x;
    double player_y = player.position.y;
    double angle = atan2(enemy_y - player_y, enemy_x - player_x);
    printf("Angle calculé pour tirer sur l'ennemi : %.2f radians\n", angle);
    return angle;
}

/**
 * @brief Détecte l'ennemi le plus proche et effectue un tir sur lui.
 * 
 * Cette fonction utilise le radar pour scanner les ennemis proches du joueur.
 * Elle identifie l'ennemi le plus proche, calcule l'angle de tir et déclenche le tir.
 * 
 * @param connection Pointeur vers la connexion au serveur de jeu.
 * @param player_x Position X du joueur.
 * @param player_y Position Y du joueur.
 * @param detection_radius Rayon de détection pour trouver les ennemis.
 */
void detect_and_shoot_nearest_enemy(BC_Connection *connection, float player_x, float player_y, float detection_radius) {
    int count;
    BC_PlayerData player = bc_get_player_data(connection);
    int my_id = player.id;
    
    /** @brief Tableau contenant les objets détectés par le radar */
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
    } else {
        printf("Aucun ennemi détecté\n");
    }

    free(objects);
}
