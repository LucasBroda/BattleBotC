#ifndef MOVE_H
#define MOVE_H

#include "battle_c.h"

void move_player(BC_Connection *connection, double x, double y, double z, float detection_perimeter, float player_x, float player_y);

#endif