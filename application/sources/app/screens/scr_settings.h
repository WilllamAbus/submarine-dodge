#ifndef __SCR_SETTINGS_H__
#define __SCR_SETTINGS_H__

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
#include "buzzer.h"
#include "eeprom.h"

#define EEPROM_SETTINGS_ADDR    0x20
#define SETTINGS_MENU_MAX       4

typedef enum {
    SPEED_EASY   = 0,
    SPEED_NORMAL = 1,
    SPEED_HARD   = 2
} game_speed_t;

typedef struct {
    uint8_t     sound_on;
    game_speed_t speed;
} settings_t;

extern settings_t game_settings;

void settings_load();
void settings_save();
void settings_apply_speed();

extern view_dynamic_t dyn_view_settings;
extern view_screen_t scr_settings;
extern void scr_settings_handle(ak_msg_t* msg);

#endif //__SCR_SETTINGS_H__