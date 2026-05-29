#include "scr_loading.h"
#include "sub_game_common.h"
#include "scr_submarine_game.h"

#define LOADING_INTERVAL    (80)   /* 80ms mỗi tick */
#define LOADING_MAX         (20)   /* 20 tick = 1.6 giây */
#define LOADING_BAR_WIDTH   (100)
#define LOADING_BAR_X       (14)
#define LOADING_BAR_Y       (38)
#define LOADING_BAR_H       (8)

static uint8_t loading_progress = 0;

/* ==================== VIEW ==================== */
static void view_scr_loading();

view_dynamic_t dyn_view_loading = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_loading
};

view_screen_t scr_loading = {
    &dyn_view_loading,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_loading() {
    /* Title */
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(20, 8);
    view_render.print("Submarine Dodge");

    /* Loading text */
    view_render.setCursor(42, 24);
    view_render.print("Loading...");

    /* Khung thanh loading */
    view_render.drawRect(LOADING_BAR_X, LOADING_BAR_Y,
                         LOADING_BAR_WIDTH, LOADING_BAR_H, WHITE);

    /* Thanh loading fill */
    uint8_t fill_width = (loading_progress * LOADING_BAR_WIDTH) / LOADING_MAX;
    if (fill_width > 0) {
        view_render.fillRect(LOADING_BAR_X, LOADING_BAR_Y,
                             fill_width, LOADING_BAR_H, WHITE);
    }

    /* Phần trăm */
    uint8_t percent = (loading_progress * 100) / LOADING_MAX;
    view_render.setCursor(54, 50);
    view_render.print(percent);
    view_render.print("%");
}

/* ==================== HANDLER ==================== */
void scr_loading_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    case SCREEN_ENTRY: {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        loading_progress = 0;
        timer_set(AC_TASK_DISPLAY_ID,
                  AC_DISPLAY_LOADING_UPDATE,
                  LOADING_INTERVAL,
                  TIMER_PERIODIC);
    }
    break;

    case AC_DISPLAY_LOADING_UPDATE: {
        loading_progress++;
        if (loading_progress >= LOADING_MAX) {
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_LOADING_UPDATE);
            game_buzzer_play(tones_3beep);
            SCREEN_TRAN(scr_submarine_game_handle, &scr_submarine_game);
        }
    }
    break;

    default:
        break;
    }
}