#include "battle_c.h"
#include "stdio.h"
#include "stdlib.h"

// Fonction permettant de convertir les types d'object de l'enum en chaine de caractère
char* ConvertObjectTypeToString(enum BC_ObjectType type) {
    switch (type) {
        case OT_PLAYER: return "PLAYER";
        case OT_WALL:   return "WALL";
        case OT_BOOST:  return "BOOST";
        default:        return "UNKNOWN";
    }
}

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
 
// Fonction permettant de bouger le joueur
void move_player(BC_Connection *connection, double x, double y, double z){
  bc_set_speed(connection, x, y, z);
  printf("Le joueur a bougé à la position x: %.2f, y: %.2f, z: %.2f\n", x, y, z);
}

// Fonction faisant office de radar, permet donc de piger les objets proches du joueur et d'afficher leurs informations
void radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters, float meters_to_pixels){
  float detection_radius_pixels = detection_radius_meters * meters_to_pixels;
  
  BC_List *list = bc_radar_ping(connection);
  BC_List *current = list;
  while (current != NULL){
    BC_MapObject *object = bc_ll_value(current);
    float distance = sqrt(pow(object->position.x - player_x, 2) + pow(object->position.y - player_y, 2));
    if (distance <= detection_radius_pixels) {
      printf("----------------------Nouveau scan----------------------\n");
      printf("ID : %d\n", object->id);
      printf("Type : %s\n", ConvertObjectTypeToString(object->type));
      printf("Vie : %d\n", object->health);
      printf("Position x: %.2f\n", object->position.x);
      printf("Position y: %.2f\n", object->position.y);
    }
    current = bc_ll_next(current);
  }
  bc_ll_free(list);
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
    printf("Information sur le monde courant\n");
    bc_get_world_info(conn);

    // Affiche les données du joueur courant
    printf("Affichage des données du joueur courant\n");
    print_data_current_player(conn);

    // Permet de bouger le joueur
    move_player(conn, 1, 1, 1);

    float player_x = 100.0f; 
    float player_y = 200.0f;
    float detection_radius_meters = 10.0f;
    float meters_to_pixels = 50.0f;
    // Radar
    // while(true){
    radar(conn, player_x, player_y, detection_radius_meters, meters_to_pixels);
    // }

  return EXIT_SUCCESS;
}
