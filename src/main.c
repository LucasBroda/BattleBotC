#include "battle_c.h"

#include "stdio.h"
#include "stdlib.h"

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

  return EXIT_SUCCESS;
}
