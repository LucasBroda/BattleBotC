#include "battle_c.h"
#include <cstdlib>
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
  BC_Connection *conn = bc_connect("127.0.0.1", 8080);

  bc_get_world_info(conn);
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }

  bc_set_speed(conn, 1.2, 0.4, 0);
  {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }
  BC_PlayerData data = bc_get_player_data(conn);

  std::cout << "x = " << data.position.x << ", y =" << data.position.y
            << std::endl;
  BC_List *list = bc_radar_ping(conn);

  do
  {
    BC_MapObject *map_object = (BC_MapObject *)bc_ll_value(list);
    std::cout << "map_object x=" << map_object->position.x
              << ", y=" << map_object->position.y << std::endl;
  } while (((list = bc_ll_next(list)) != nullptr));

  return EXIT_SUCCESS;
}
