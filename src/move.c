#include "radar.h"
#include "stdlib.h"
#include <string.h>
#include "stdio.h"

#define DISTANCE_CRITIQUE 2.0

// Fonction permettant de bouger le joueur
void move_player(BC_Connection *connection, double x, double y, double z, float detection_perimeter, float player_x, float player_y){
    int object_count = 0;

    // Appel de la fonction radar pour vérifier les obstacles
    ObjectInfo* objects = radar(connection, player_x, player_y, detection_perimeter, &object_count);

    ObjectInfo* boost = NULL;

    int obstacle_detected = 0;
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

    // Si un obstacle est détecté, inverser la direction
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

    bc_set_speed(connection, x, y, z);
    printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
    fflush(stdout);
}