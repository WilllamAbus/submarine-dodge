#ifndef __SCR_RANKING_H__
#define __SCR_RANKING_H__

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

#define RANKING_MAX         3
#define EEPROM_RANKING_ADDR 0x10

typedef struct {
    uint32_t scores[RANKING_MAX];
} ranking_t;

extern ranking_t ranking;

void ranking_update(uint32_t new_score);
void ranking_load();
void ranking_save();

extern view_dynamic_t dyn_view_ranking;
extern view_screen_t scr_ranking;
extern void scr_ranking_handle(ak_msg_t* msg);

#endif //__SCR_RANKING_H__