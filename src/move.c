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


#define DISTANCE_CRITIQUE 2.0

// Fonction permettant de bouger le joueur
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


    ObjectInfo* boost = NULL;

    /** @brief Indicateur de la présence d'un obstacle */

    int obstacle_detected = 0;

    // Vérification de la présence d'obstacles (murs)
    for (int i = 0; i < object_count; i++) {

        // Définitions des distances aux objets
        float distance_x = objects[i].position_x - player_x;
        float distance_y = objects[i].position_y - player_y;
        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        //Détection d'un mur
        if (strcmp(objects[i].type, "WALL") == 0 && distance < DISTANCE_CRITIQUE) {
            if ((distance_x * x > 0) && (distance_y * y > 0)) {
                obstacle_detected = 1;
                break;
            }
        }


        //Détection d'un boost
        if (strcmp(objects[i].type, "BOOST") == 0){
            if (boost == NULL || sqrt(distance_x * distance_x + distance_y * distance_y) < sqrt((boost->position_x - player_x) * (boost->position_x - player_x) + (boost->position_y - player_y) * (boost->position_y - player_y))) {
                boost = &objects[i];
            }
        }
    }

    // Libération de la mémoire allouée pour les objets détectés
    free(objects);

    // Si un obstacle est détecté, inversion de la direction
    if (obstacle_detected) {
        x = -x;
        y = -y;
        printf("Obstacle détecté à moins de %.2f unités, changement de direction\n", DISTANCE_CRITIQUE);
        fflush(stdout);
    }


    if (boost != NULL) {
        double direction_x = boost->position_x - player_x;
        double direction_y = boost->position_y - player_y;
        double magnitude = sqrt(direction_x * direction_x + direction_y * direction_y);
        
        // Normaliser la direction
        if (magnitude > 0) {
            x = (direction_x / magnitude) * fabs(x);
            y = (direction_y / magnitude) * fabs(y);
        }

        printf("Boost détecté, nouvelle direction vers x: %.2f, y: %.2f\n", x, y);
        fflush(stdout);
    }

    // Libérer la mémoire allouée
    free(objects);


    // Application du déplacement
    bc_set_speed(connection, x, y, z);
    printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
    fflush(stdout);
}