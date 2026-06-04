#ifndef __SUB_GAME_BOSS_H__
#define __SUB_GAME_BOSS_H__

#include "sub_game_submarine.h"


#define BOSS_WIDTH              32
#define BOSS_HEIGHT             16
#define BOSS_HP_EASY    50
#define BOSS_HP_NORMAL  100
#define BOSS_HP_HARD    150
#define BOSS_BULLET_MAX         4
#define BOSS_SCORE_TRIGGER      200

/* Time-based thay thế */
#define BOSS_SPEED_PPS          20      /* pixel/giây */
#define BOSS_BULLET_SPEED_PPS   80      /* pixel/giây */
#define BOSS_SHOOT_INTERVAL_MS  500     /* bắn mỗi 500ms */

typedef struct {
    int8_t   x;
    int8_t   y;
    int16_t  y_fp;          /* fixed point y */
    int8_t   dir;
    uint8_t  active;
    int8_t   hp;
} boss_t;

typedef struct {
    int16_t  x_fp;          /* fixed point x */
    int8_t   x;
    int8_t   y;
    uint8_t  active;
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