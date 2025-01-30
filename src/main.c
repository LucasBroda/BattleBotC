#include "battle_c.h"

#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>

// Fonction permettant d'afficher les données du joueur courant
void print_data_current_player(BC_Connection *connection){
  BC_PlayerData player = bc_get_player_data(connection);
    printf("ID : %d\n", player.id);
    printf("Vie du joueur : %d\n", player.health);
    printf("Armure du joueur : %d\n", player.armor);
    printf("Score : %d\n", player.score);
    printf("Position x: %.2f\n", player.position.x);
    printf("Position y: %.2f\n", player.position.y);
}

double calculate_shoot_angle(BC_Connection *connection, double enemy_x, double enemy_y) {
    // Récupère la position actuelle du joueur
    BC_PlayerData player = bc_get_player_data(connection);
    double player_x = player.position.x;
    double player_y = player.position.y;

    // Calcule l'angle en utilisant atan2 (donne l'angle en radians)
    double angle = atan2(enemy_y - player_y, enemy_x - player_x);

    printf("Angle calculé pour tirer sur l'ennemi : %.2f radians\n", angle);
    return angle;
}

 
// Fonction permettant de bouger le joueur
void move_player(BC_Connection *connection, double x, double y, double z){
  bc_set_speed(connection, x, y, z);
  printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
}

// Fonction faisant office de radar, permet donc de piger les objets proches du joueur et d'afficher leurs informations
void radar(BC_Connection *connection){
  BC_List *list = bc_radar_ping(connection);
  BC_List *current = list;
  while (current != NULL){
    BC_MapObject *object = bc_ll_value(current);
    printf("ID : %d\n", object->id);
    printf("Type : %d\n", object->type);
    printf("Vie : %d\n", object->health);
    printf("Position x: %.2f\n", object->position.x);
    printf("Position y: %.2f\n", object->position.y);
    printf("Position z: %.2f\n", object->position.z);
    printf("Vitesse x: %.2f\n", object->speed.x);
    printf("Vitesse y: %.2f\n", object->speed.y);
    printf("Vitesse z: %.2f\n", object->speed.z);
    current = bc_ll_next(current);
  }
  bc_ll_free(list);
}

// Fonction permettant au joueur de tirer
void shoot(BC_Connection *connection, double angle) {
    // Appelle la fonction bc_shoot pour effectuer le tir
    BC_ShootResult result = bc_shoot(connection, angle);

    // Affiche les résultats du tir
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

int main(int argc, char *argv[])
{

  BC_Connection *conn = bc_connect("5.135.136.236", 8080);

  if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }
    printf("Connecté au serveur avec succès letsgo !\n");

    // Information sur le monde courant
    // printf("Information sur le monde courant\n");
    // bc_get_world_info(conn);
    
    // Affiche les données du joueur courant
    printf("Affichage des données du joueur courant\n");
    print_data_current_player(conn);

    // Permet de bouger le joueur
    move_player(conn, 1, 1, 1);

    // Tir
    // Boucle qui tire toutes les 10 secondes
    double angle = 1.66; // Angle fixe de tir (90 degrés)
    printf("Lancement de la boucle de tir automatique toutes les 10 secondes...\n");
    
    for (int i = 0; i < 5; i++) { // Boucle de 5 tirs, modifie ou enlève la condition pour une boucle infinie
        printf("\n[Tour %d] Préparation du tir...\n", i + 1);
        shoot(conn, angle); // Effectue le tir
        sleep(10); // Attente de 10 secondes
    }

    printf("Fin de la boucle de tir automatique.\n");

    // Radar
    printf("Radar\n");
    radar(conn);

  return EXIT_SUCCESS;
}
