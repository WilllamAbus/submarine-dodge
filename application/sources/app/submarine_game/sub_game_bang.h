#ifndef __SUB_GAME_BANG_H__
#define __SUB_GAME_BANG_H__

#include "sub_game_submarine.h"

#define BANG_WIDTH      16
#define BANG_HEIGHT     16
#define BANG_DURATION   5    /* Số frame hiển thị */
#define BANG_MAX        3

typedef struct {
    int8_t  x;
    int8_t  y;
    uint8_t active;
    uint8_t timer;   /* Đếm ngược frame */
} bang_t;

extern bang_t bangs[BANG_MAX];

void sub_game_bang_setup();
void sub_game_bang_spawn(int8_t x, int8_t y);
void sub_game_bang_update();
void sub_game_bang_draw();
void sub_game_bang_handle(ak_msg_t* msg);
#endif /* __SUB_GAME_BANG_H__ */