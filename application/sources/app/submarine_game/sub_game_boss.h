#ifndef __SUB_GAME_BOSS_H__
#define __SUB_GAME_BOSS_H__

#include "sub_game_submarine.h"

#define BOSS_WIDTH          32
#define BOSS_HEIGHT         16
#define BOSS_HP_MAX         100
#define BOSS_SPEED          1
#define BOSS_BULLET_MAX     4
#define BOSS_BULLET_SPEED   4
#define BOSS_SCORE_TRIGGER  200   /* Xuất hiện khi đạt 200 điểm */

typedef struct {
    int8_t  x;
    int8_t  y;
    int8_t  dir;
    uint8_t active;
    uint8_t hp;
} boss_t;

typedef struct {
    int8_t  x;
    int8_t  y;
    uint8_t active;
} boss_bullet_t;

extern boss_t boss;
extern boss_bullet_t boss_bullets[BOSS_BULLET_MAX];

void sub_game_boss_setup();
void sub_game_boss_update();
void sub_game_boss_draw();
void sub_game_boss_draw_hp();
uint8_t sub_game_boss_hit_by_torpedo();
uint8_t sub_game_boss_bullet_hit_submarine();
void sub_game_boss_handle(ak_msg_t* msg);

#endif /* __SUB_GAME_BOSS_H__ */