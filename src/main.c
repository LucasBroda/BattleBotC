#include "battle_c.h"

#include "stdio.h"
#include "stdlib.h"

feature/CreationJoueur
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
 main

  BC_Connection *conn = bc_connect("5.135.136.236", 8080);

  bc_get_world_info(conn);

  bc_set_speed(conn, 1.2, 0.4, 0);

  BC_PlayerData data = bc_get_player_data(conn);

  BC_List *list = bc_radar_ping(conn);

feature/CreationJoueur
  create_player_object(1, data.position, data.speed, data.health, data.score, data.armor);

  do {
main
    BC_MapObject *map_object = (BC_MapObject *)bc_ll_value(list);
    printf("map_object x = %d, y = %d", map_object->position.x,
           map_object->position.y);

  } while (((list = bc_ll_next(list)) != NULL));
  return EXIT_SUCCESS;
}
