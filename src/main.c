#include "battle_c.h"

#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{

  BC_Connection *conn = bc_connect("5.135.136.236", 8080);

  bc_get_world_info(conn);

  bc_set_speed(conn, 1.2, 0.4, 0);

  BC_PlayerData data = bc_get_player_data(conn);

  BC_List *list = bc_radar_ping(conn);

  do
  {
    BC_MapObject *map_object = (BC_MapObject *)bc_ll_value(list);
    printf("map_object x = %d, y = %d", map_object->position.x,
           map_object->position.y);

  } while (((list = bc_ll_next(list)) != NULL));
  return EXIT_SUCCESS;
}
