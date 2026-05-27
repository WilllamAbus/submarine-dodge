#ifndef __SCR_LOADING_H__
#define __SCR_LOADING_H__

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

extern view_dynamic_t dyn_view_loading;
extern view_screen_t scr_loading;
extern void scr_loading_handle(ak_msg_t* msg);

#endif //__SCR_LOADING_H__