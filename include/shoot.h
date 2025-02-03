#ifndef SHOOT_H
#define SHOOT_H

#include "battle_c.h"

void shoot(BC_Connection *connection, double angle);



double calculate_shoot_angle(BC_Connection *connection, double enemy_x, double enemy_y);



void detect_and_shoot_nearest_enemy(BC_Connection *connection, float player_x, float player_y, float detection_radius);


#endif