#ifndef __SUB_GAME_SUBMARINE_H__
#define __SUB_GAME_SUBMARINE_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

/* Kích thước tàu ngầm */
#define SUBMARINE_WIDTH     16
#define SUBMARINE_HEIGHT    8

/* Giới hạn di chuyển theo trục Y */
#define SUBMARINE_Y_MIN     2
#define SUBMARINE_Y_MAX     54

/* Tốc độ di chuyển */
#define SUBMARINE_STEP      4

/* Vị trí cố định theo trục X */
#define SUBMARINE_X         10

/* Struct tàu ngầm */
typedef struct {
    int8_t x;
    int8_t y;
    uint8_t is_alive;
} submarine_t;

/* Biến toàn cục */
extern submarine_t submarine;

/* Hàm xử lý */
void sub_game_submarine_setup();
void sub_game_submarine_up();
void sub_game_submarine_down();
void sub_game_submarine_reset();
void sub_game_submarine_draw();

#endif /* __SUB_GAME_SUBMARINE_H__ */