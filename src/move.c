/**
 * @file move.c
 * @brief Implémentation du déplacement du joueur en évitant les obstacles détectés par le radar.
 *
 * Ce fichier contient la fonction permettant de déplacer un joueur dans l'environnement de jeu,
 * en prenant en compte les obstacles détectés via un radar.
 *
 */

#include "radar.h"   /**< Gestion du radar et détection d'obstacles */
#include "stdlib.h"
#include <string.h>
#include "stdio.h"

/**
 * @brief Déplace le joueur tout en évitant les obstacles détectés.
 *
 * Cette fonction ajuste la direction du joueur en fonction des obstacles détectés
 * par le radar. Si un mur est trouvé sur la trajectoire prévue, la direction est inversée.
 *
 * @param connection Pointeur vers la connexion au serveur.
 * @param x Coordonnée de déplacement sur l'axe X.
 * @param y Coordonnée de déplacement sur l'axe Y.
 * @param z Coordonnée de déplacement sur l'axe Z.
 * @param detection_perimeter Rayon de détection du radar.
 * @param player_x Position actuelle du joueur sur l'axe X.
 * @param player_y Position actuelle du joueur sur l'axe Y.
 */
void move_player(BC_Connection *connection, double x, double y, double z, float detection_perimeter, float player_x, float player_y) {
    /** @brief Nombre d'objets détectés par le radar */
    int object_count = 0;

    /** @brief Liste des objets détectés par le radar */
    ObjectInfo* objects = radar(connection, player_x, player_y, detection_perimeter, &object_count);

    /** @brief Indicateur de la présence d'un obstacle */
    int obstacle_detected = 0;

    // Vérification de la présence d'obstacles (murs)
    for (int i = 0; i < object_count; i++) {
        if (strcmp(objects[i].type, "WALL") == 0) {
            float distance_x = objects[i].position_x - player_x;
            float distance_y = objects[i].position_y - player_y;
            if ((distance_x * x > 0) && (distance_y * y > 0)) {
                obstacle_detected = 1;
                break;
            }
        }
    }

    // Libération de la mémoire allouée pour les objets détectés
    free(objects);

    // Si un obstacle est détecté, inversion de la direction
    if (obstacle_detected) {
        x = -x;
        y = -y;
        z = -z;
        printf("Obstacle détecté, changement de direction\n");
        fflush(stdout);
    }

    // Application du déplacement
    bc_set_speed(connection, x, y, z);
    printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
    fflush(stdout);
}