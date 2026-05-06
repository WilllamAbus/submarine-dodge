#ifndef __SUB_GAME_TORPEDO_H__
#define __SUB_GAME_TORPEDO_H__

#include "sub_game_submarine.h"

#define TORPEDO_WIDTH       8
#define TORPEDO_HEIGHT      3
#define TORPEDO_SPEED       4
#define TORPEDO_MAX         3    /* Tối đa 3 ngư lôi cùng lúc */

typedef struct {
    int8_t x;
    int8_t y;
    int8_t dir;      /* +1: bay phải, -1: bay trái */
    uint8_t active;  /* 1: đang bay, 0: không hoạt động */
} torpedo_t;

extern torpedo_t torpedoes[TORPEDO_MAX];

void sub_game_torpedo_setup();
void sub_game_torpedo_update();
void sub_game_torpedo_draw();
uint8_t sub_game_torpedo_hit(int8_t x, int8_t y, uint8_t w, uint8_t h);
void sub_game_torpedo_handle(ak_msg_t* msg);
#endif /* __SUB_GAME_TORPEDO_H__ */