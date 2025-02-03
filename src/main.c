#include "battle_c.h"
#include "radar.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <math.h>


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

    float player_x = bc_get_player_data(conn).position.x; 
    float player_y = bc_get_player_data(conn).position.y;
    float detection_radius_meters = 10.0f;

    // Radar
    int player_count = 0;
    radar(conn, player_x, player_y, detection_radius_meters, &player_count);
    printf("Fin du radar\n");
    fflush(stdout);
    // Permet de bouger le joueur
    while(1){
      move_player(conn, 2, 2, 2, player_x, player_y, detection_radius_meters);
    }

  return EXIT_SUCCESS;
}