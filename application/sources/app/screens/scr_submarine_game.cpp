#include "scr_submarine_game.h"

/* ==================== TIMER INTERVAL ==================== */
#define SUB_GAME_UPDATE_INTERVAL    (150)

/* ==================== GAME STATE ==================== */
typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_OVER
} game_state_t;

static game_state_t game_state;
static uint32_t score;

/* ==================== VIEW ==================== */
static void view_scr_submarine_game();

view_dynamic_t dyn_view_submarine_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_submarine_game
};

view_screen_t scr_submarine_game = {
    &dyn_view_submarine_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

/* ==================== VIEW RENDER ==================== */
static void view_scr_submarine_game() {
    /* Vẽ border trên và dưới */
    view_render.drawFastHLine(0, 0, LCD_WIDTH, WHITE);
    view_render.drawFastHLine(0, LCD_HEIGHT - 1, LCD_WIDTH, WHITE);

    /* Vẽ tàu ngầm */
    sub_game_submarine_draw();

    /* Vẽ score */
    view_render.setCursor(90, 2);
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.print(score);
}

/* ==================== SCREEN HANDLER ==================== */
void scr_submarine_game_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    case SCREEN_ENTRY: {
        APP_DBG_SIG("SCREEN_ENTRY\n");

        /* Reset game */
        game_state = GAME_STATE_PLAYING;
        score = 0;
        sub_game_submarine_setup();

        /* Bắt đầu timer update game */
        timer_set(AC_TASK_DISPLAY_ID,
                  AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE,
                  SUB_GAME_UPDATE_INTERVAL,
                  TIMER_PERIODIC);
    }
    break;

    case AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE: {
        if (game_state == GAME_STATE_PLAYING) {
            score++;
        }
    }
    break;

    case AC_DISPLAY_BUTON_UP_RELEASED: {
        APP_DBG_SIG("AC_DISPLAY_BUTON_UP_RELEASED\n");
        if (game_state == GAME_STATE_PLAYING) {
            sub_game_submarine_up();
        }
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED: {
        APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_RELEASED\n");
        if (game_state == GAME_STATE_PLAYING) {
            sub_game_submarine_down();
        }
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED: {
        APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
        /* Thoát về màn hình idle */
        timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
        SCREEN_TRAN(scr_idle_handle, &scr_idle);
    }
    break;

    default:
        break;
    }
}