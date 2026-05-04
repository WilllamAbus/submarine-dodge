#ifndef __SUB_GAME_OBSTACLE_H__
#define __SUB_GAME_OBSTACLE_H__

#include "sub_game_submarine.h"

#define OBSTACLE_WIDTH      8
#define OBSTACLE_HEIGHT     8
#define OBSTACLE_SPEED      2
#define OBSTACLE_MAX        4    /* Tối đa 4 chướng ngại vật */

typedef struct {
    int8_t x;
    int8_t y;
    int8_t dir;      /* +1: từ phải sang, -1: từ trái sang */
    uint8_t active;
} obstacle_t;

extern obstacle_t obstacles[OBSTACLE_MAX];

void sub_game_obstacle_setup();
void sub_game_obstacle_spawn(uint8_t from_right);
void sub_game_obstacle_update();
void sub_game_obstacle_draw();
uint8_t sub_game_obstacle_hit_submarine();

#endif /* __SUB_GAME_OBSTACLE_H__ */