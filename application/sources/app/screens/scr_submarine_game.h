#ifndef __SCR_SUBMARINE_GAME_H__
#define __SCR_SUBMARINE_GAME_H__

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

#include "sub_game_submarine.h"
#include "sub_game_torpedo.h"
#include "sub_game_obstacle.h"
#include "sub_game_bang.h"

extern view_dynamic_t dyn_view_submarine_game;
extern view_screen_t scr_submarine_game;
extern void scr_submarine_game_handle(ak_msg_t* msg);

#endif //__SCR_SUBMARINE_GAME_H__