#include "radar.h"
#include "stdlib.h"
#include <string.h>
#include "stdio.h"

// Fonction permettant de bouger le joueur
void move_player(BC_Connection *connection, double x, double y, double z, float detection_perimeter, float player_x, float player_y){
    int object_count = 0;

    // Appel de la fonction radar pour vérifier les obstacles
    ObjectInfo* objects = radar(connection, player_x, player_y, detection_perimeter, &object_count);

    int obstacle_detected = 0;
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

    // Libérer la mémoire allouée
    free(objects);

    // Si un obstacle est détecté, inverser la direction
    if (obstacle_detected) {
        x = -x;
        y = -y;
        z = -z;
        printf("Obstacle détecté, changement de direction\n");
        fflush(stdout);
    }

    bc_set_speed(connection, x, y, z);
    printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
    fflush(stdout);
}