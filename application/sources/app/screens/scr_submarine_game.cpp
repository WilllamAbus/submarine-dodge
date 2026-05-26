#include "scr_submarine_game.h"

#define SB_GAME_SPAWN_INTERVAL (5)

typedef enum
{
    GAME_STATE_PLAYING,
    GAME_STATE_OVER
} game_state_t;

static game_state_t game_state;
uint32_t sb_game_score;

/* ==================== VIEW ==================== */
static void view_scr_submarine_game();

view_dynamic_t dyn_view_submarine_game = {
    {.item_type = ITEM_TYPE_DYNAMIC},
    view_scr_submarine_game};

view_screen_t scr_submarine_game = {
    &dyn_view_submarine_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void draw_water_environment()
{
    /* Vẽ mặt nước phía trên */
    view_render.drawFastHLine(0, 8, LCD_WIDTH, WHITE);

    /* Vẽ sóng nước */
    for (uint8_t x = 0; x < LCD_WIDTH; x += 8)
    {
        view_render.drawPixel(x + 2, 6, WHITE);
        view_render.drawPixel(x + 4, 5, WHITE);
        view_render.drawPixel(x + 6, 6, WHITE);
    }

    /* Vẽ bong bóng */
    static uint8_t bubble_y = 50;
    bubble_y--;
    if (bubble_y < 10)
        bubble_y = 55;
    view_render.drawPixel(20, bubble_y, WHITE);
    view_render.drawPixel(60, bubble_y + 5, WHITE);
    view_render.drawPixel(100, bubble_y - 3, WHITE);

    /* Vẽ đáy biển */
    view_render.drawFastHLine(0, LCD_HEIGHT - 8, LCD_WIDTH, WHITE);
    for (uint8_t x = 0; x < LCD_WIDTH; x += 6)
    {
        view_render.drawPixel(x + 1, LCD_HEIGHT - 7, WHITE);
        view_render.drawPixel(x + 3, LCD_HEIGHT - 6, WHITE);
    }
}
static void view_scr_submarine_game()
{
    view_render.drawFastHLine(0, 0, LCD_WIDTH, WHITE);
    view_render.drawFastHLine(0, LCD_HEIGHT - 1, LCD_WIDTH, WHITE);
    draw_water_environment();
    if (game_state == GAME_STATE_PLAYING)
    {
        sub_game_submarine_draw();
        sub_game_torpedo_draw();
        sub_game_obstacle_draw();
        sub_game_bang_draw();
        sub_game_obstacle_draw_bullets();

        view_render.setCursor(85, 2);
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.print(sb_game_score);
    }
    else
    {
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);

        /* GAME OVER - căn giữa (9 ký tự x 6px = 54px, start = (128-54)/2 = 37) */
        view_render.setCursor(37, 15);
        view_render.print("GAME OVER");

        /* Score - căn giữa */
        view_render.setCursor(37, 28);
        view_render.print("Score: ");
        view_render.print(sb_game_score);

        /* MODE: menu - căn giữa (10 ký tự x 6px = 60px, start = (128-60)/2 = 34) */
        view_render.setCursor(25, 45);
        view_render.print("MODE to menu");
    }
}
static void handle_game_over()
{
    sub_game_bang_spawn(submarine.x, submarine.y);
    BUZZER_PlayTones(tones_3beep);
    game_state = GAME_STATE_OVER;
    timer_remove_attr(AC_TASK_DISPLAY_ID, SB_GAME_TIME_TICK);
}
/* ==================== HANDLER ==================== */
void scr_submarine_game_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {

    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        game_state = GAME_STATE_PLAYING;
        sb_game_score = 0;

        /* Setup tất cả object */
        task_post_pure_msg(SB_GAME_SUBMARINE_ID, SB_GAME_SUBMARINE_SETUP);
        task_post_pure_msg(SB_GAME_TORPEDO_ID, SB_GAME_TORPEDO_SETUP);
        task_post_pure_msg(SB_GAME_OBSTACLE_ID, SB_GAME_OBSTACLE_SETUP);
        task_post_pure_msg(SB_GAME_BANG_ID, SB_GAME_BANG_SETUP);

        /* Bắt đầu timer game loop */
        timer_set(AC_TASK_DISPLAY_ID,
                  SB_GAME_TIME_TICK,
                  SB_GAME_TIME_TICK_INTERVAL,
                  TIMER_PERIODIC);
    }
    break;

    case SB_GAME_TIME_TICK:
    {
        if (game_state != GAME_STATE_PLAYING)
            break;

        /* Bỏ tự động tăng điểm - xóa dòng sb_game_score++ */

        /* Update vị trí */
        task_post_pure_msg(SB_GAME_TORPEDO_ID, SB_GAME_TORPEDO_RUN);
        task_post_pure_msg(SB_GAME_OBSTACLE_ID, SB_GAME_OBSTACLE_RUN);
        task_post_pure_msg(SB_GAME_BANG_ID, SB_GAME_BANG_UPDATE);
        task_post_pure_msg(SB_GAME_SUBMARINE_ID, SB_GAME_SUBMARINE_UPDATE);
        /* +10 điểm: torpedo bắn trúng obstacle */
        for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
        {
            if (!obstacles[i].active)
                continue;
            if (sub_game_torpedo_hit(obstacles[i].x,
                                     obstacles[i].y,
                                     OBSTACLE_WIDTH,
                                     OBSTACLE_HEIGHT))
            {
                obstacles[i].active = 0;
                sub_game_bang_spawn(obstacles[i].x, obstacles[i].y);
                sb_game_score += 10;
            }
        }

        /* Kiểm tra đạn địch trúng tàu player , */
        if (sub_game_obstacle_hit_submarine() || sub_game_enemy_bullet_hit_submarine())
        {
            handle_game_over();
        }
    }
    break;

    case AC_DISPLAY_BUTON_UP_RELEASED:
    {
        if (game_state == GAME_STATE_PLAYING)
        {
            task_post_pure_msg(SB_GAME_SUBMARINE_ID, SB_GAME_SUBMARINE_UP);
        }
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED:
    {
        if (game_state == GAME_STATE_PLAYING)
        {
            task_post_pure_msg(SB_GAME_SUBMARINE_ID, SB_GAME_SUBMARINE_DOWN);
        }
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED:
    {
        APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
        if (game_state == GAME_STATE_PLAYING)
        {
            /* Bắn torpedo */
            task_post_pure_msg(SB_GAME_TORPEDO_ID, SB_GAME_TORPEDO_SHOOT);
        }
        else
        {
            /* Restart game */
            timer_remove_attr(AC_TASK_DISPLAY_ID, SB_GAME_TIME_TICK);
            SCREEN_TRAN(scr_main_menu_handle, &scr_main_menu);
        }
    }
    break;

    default:
        break;
    }
}