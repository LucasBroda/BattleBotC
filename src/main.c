#include "battle_c.h"

#include "stdio.h"
#include "stdlib.h"

void print_data_current_player(BC_Connection *connection){
  BC_PlayerData player = bc_get_player_data(connection);
    printf("ID : %d\n", player.id);
    printf("Health : %d\n", player.health);
    printf("Armor : %d\n", player.armor);
    printf("Score : %d\n", player.score);
    printf("Position x: %.2f\n", player.position.x);
    printf("Position y: %.2f\n", player.position.y);
}

BC_MapObject create_player_object(int id, BC_Vector3 position, BC_Vector3 speed, int health, int score, int armor) {
    BC_PlayerData player_data;
    player_data.id = id;
    player_data.position = position;
    player_data.speed = speed;
    player_data.health = health;
    player_data.score = score;
    player_data.armor = armor;
    player_data.is_dead = false;
    
    BC_MapObject player_object;
    player_object.type = OT_PLAYER;
    player_object.id = id;
    player_object.position = position;
    player_object.speed = speed;
    player_object.health = health;

    return player_object;
}

int main(int argc, char *argv[])
{

  BC_Connection *conn = bc_connect("5.135.136.236", 8080);

  if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }
    printf("Connecté au serveur avec succès !\n");

    print_data_current_player(conn);

  return EXIT_SUCCESS;
}
