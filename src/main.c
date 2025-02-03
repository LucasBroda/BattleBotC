#include "battle_c.h"
#include "radar.h"
#include "move.h"
#include "print.h"
#include "shoot.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "unistd.h"

int main(int argc, char *argv[]) {
    // Connexion au serveur
    BC_Connection *conn = bc_connect("5.135.136.236", 8080);
    if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }
    printf("Connecté au serveur avec succès !\n");
    fflush(stdout);
    
    bc_get_world_info(conn);
    print_data_current_player(conn);
    
    // Récupération des coordonnées du joueur
    float player_x = bc_get_player_data(conn).position.x; 
    float player_y = bc_get_player_data(conn).position.y;
    float detection_radius_meters = 10.0f;


    while (1) {
        int playercount = 0;
        radar(conn, player_x, player_y, detection_radius_meters, &playercount);
        print_data_current_player(conn);
        detect_and_shoot_nearest_enemy(conn, player_x, player_y, detection_radius_meters);
        usleep(1000000); // Pause de 1 seconde

        // Déplacement du joueur
        move_player(conn, 2.0, 2.0, 2.0, detection_radius_meters, player_x, player_y);
        player_x = bc_get_player_data(conn).position.x;
        player_y = bc_get_player_data(conn).position.y;
    
    }

    return EXIT_SUCCESS;
}
