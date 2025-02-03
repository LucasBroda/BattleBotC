#ifndef RADAR_H
#define RADAR_H

#include "battle_c.h"

// Définition de la structure pour stocker les informations des objets
typedef struct {
    char* type;
    float position_x;
    float position_y;
    float id;
} ObjectInfo;

char* ConvertObjectTypeToString(enum BC_ObjectType type);

// Fonction permettant de scanner les objets autour du joueur
ObjectInfo* radar(BC_Connection *connection, float player_x, float player_y, float detection_radius_meters, int *count);

#endif
