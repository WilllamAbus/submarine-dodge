#ifndef __SUB_GAME_TORPEDO_H__
#define __SUB_GAME_TORPEDO_H__

#include "sub_game_submarine.h"
#include "game_time.h"
#define TORPEDO_WIDTH       8
#define TORPEDO_HEIGHT      3
#define TORPEDO_SPEED       4
#define TORPEDO_MAX         5    /* Tối đa 3 ngư lôi cùng lúc */

typedef struct {
    int32_t x_fp;
    int16_t y;
    int8_t dir;
    uint8_t active;
} torpedo_t;

extern torpedo_t torpedoes[TORPEDO_MAX];

void sub_game_torpedo_setup();
void sub_game_torpedo_update();
void sub_game_torpedo_draw();
uint8_t sub_game_torpedo_hit(int8_t x, int8_t y, uint8_t w, uint8_t h);
void sub_game_torpedo_handle(ak_msg_t* msg);
void sub_game_torpedo_shoot();
#endif /* __SUB_GAME_TORPEDO_H__ */