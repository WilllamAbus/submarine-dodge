#ifndef __SCR_MAIN_MENU_H__
#define __SCR_MAIN_MENU_H__

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

#define NUMBER_MENU_ITEMS   (2)

extern view_dynamic_t dyn_view_main_menu;
extern view_screen_t scr_main_menu;
extern void scr_main_menu_handle(ak_msg_t* msg);

#endif //__SCR_MAIN_MENU_H__