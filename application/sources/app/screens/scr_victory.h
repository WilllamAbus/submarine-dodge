#ifndef __SCR_VICTORY_H__
#define __SCR_VICTORY_H__

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

extern uint32_t victory_score;
extern view_dynamic_t dyn_view_victory;
extern view_screen_t scr_victory;
extern void scr_victory_handle(ak_msg_t* msg);

#endif //__SCR_VICTORY_H__