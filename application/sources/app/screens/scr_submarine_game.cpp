#include "scr_submarine_game.h"

#define SUB_GAME_UPDATE_INTERVAL    (150)
#define SUB_GAME_SPAWN_INTERVAL     (5)  /* Spawn mỗi 5 tick */

typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_OVER
} game_state_t;

static game_state_t game_state;
static uint32_t score;
static uint8_t spawn_counter;

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

static void view_scr_submarine_game() {
    /* Border */
    view_render.drawFastHLine(0, 0, LCD_WIDTH, WHITE);
    view_render.drawFastHLine(0, LCD_HEIGHT - 1, LCD_WIDTH, WHITE);

    if (game_state == GAME_STATE_PLAYING) {
        sub_game_submarine_draw();
        sub_game_obstacle_draw();
        sub_game_bang_draw();

        /* Score */
        view_render.setCursor(85, 2);
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.print(score);

    } else {
        /* Game Over screen */
        view_render.setCursor(30, 20);
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.print("GAME OVER");

        view_render.setCursor(25, 35);
        view_render.print("Score: ");
        view_render.print(score);

        view_render.setCursor(15, 50);
        view_render.print("MODE to restart");
    }
}

/* ==================== HANDLER ==================== */
void scr_submarine_game_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    case SCREEN_ENTRY: {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        game_state    = GAME_STATE_PLAYING;
        score         = 0;
        spawn_counter = 0;

        sub_game_submarine_setup();
        sub_game_obstacle_setup();
        sub_game_bang_setup();

        timer_set(AC_TASK_DISPLAY_ID,
                  AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE,
                  SUB_GAME_UPDATE_INTERVAL,
                  TIMER_PERIODIC);
    }
    break;

    case AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE: {
        if (game_state != GAME_STATE_PLAYING) break;

        score++;

        /* Spawn obstacle */
        spawn_counter++;
        if (spawn_counter >= SUB_GAME_SPAWN_INTERVAL) {
            spawn_counter = 0;
            /* Xen kẽ spawn từ trái và phải */
            sub_game_obstacle_spawn(score % 2);
        }

        /* Update vị trí */
        sub_game_obstacle_update();
        sub_game_bang_update();

        /* Kiểm tra va chạm */
        if (sub_game_obstacle_hit_submarine()) {
            sub_game_bang_spawn(submarine.x, submarine.y);
            BUZZER_PlayTones(tones_3beep);
            game_state = GAME_STATE_OVER;
            timer_remove_attr(AC_TASK_DISPLAY_ID,
                              AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
        }
    }
    break;

    case AC_DISPLAY_BUTON_UP_RELEASED: {
        if (game_state == GAME_STATE_PLAYING) {
            sub_game_submarine_up();
        }
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED: {
        if (game_state == GAME_STATE_PLAYING) {
            sub_game_submarine_down();
        }
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED: {
        APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
        timer_remove_attr(AC_TASK_DISPLAY_ID,
                          AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
        if (game_state == GAME_STATE_OVER) {
            /* Restart game */
            SCREEN_TRAN(scr_submarine_game_handle, &scr_submarine_game);
        } else {
            /* Thoát về idle */
            SCREEN_TRAN(scr_idle_handle, &scr_idle);
        }
    }
    break;

    default:
        break;
    }
}