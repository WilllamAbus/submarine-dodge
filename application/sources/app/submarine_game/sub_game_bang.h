#ifndef __SUB_GAME_BANG_H__
#define __SUB_GAME_BANG_H__

#include "sub_game_submarine.h"
#include "game_time.h"
#define BANG_WIDTH      16
#define BANG_HEIGHT     16
#define BANG_DURATION_MS    300
#define BANG_MAX        3

typedef struct {
    int8_t x;
    int8_t y;
    uint8_t active;
    uint16_t timer_ms;
} bang_t;

extern bang_t bangs[BANG_MAX];

void sub_game_bang_setup();
void sub_game_bang_spawn(int8_t x, int8_t y);
void sub_game_bang_update();
void sub_game_bang_draw();
void sub_game_bang_handle(ak_msg_t* msg);
void sub_game_bang_update_all();
#endif /* __SUB_GAME_BANG_H__ */