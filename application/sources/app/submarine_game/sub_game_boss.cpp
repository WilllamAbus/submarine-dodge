#include "sub_game_boss.h"
#include "sub_game_torpedo.h"
#include "game_time.h"
#include "scr_settings.h"
boss_t boss;
boss_bullet_t boss_bullets[BOSS_BULLET_MAX];


/* Bitmap boss 32x16px - tàu ngầm lớn */
static const uint8_t boss_bitmap[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x80,
    0x00,
    0x00,
    0x07,
    0xE0,
    0x00,
    0x01,
    0xFF,
    0xFF,
    0x80,
    0x07,
    0xFF,
    0xFF,
    0xE0,
    0x0F,
    0xFF,
    0xFF,
    0xF0,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x0F,
    0xFF,
    0xFF,
    0xF0,
    0x07,
    0xFF,
    0xFF,
    0xE0,
    0x01,
    0xFF,
    0xFF,
    0x80,
    0x00,
    0x07,
    0xE0,
    0x00,
    0x00,
    0x01,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* Bitmap trái tim boss HP 5x5px */
static const uint8_t boss_heart_bitmap[] = {
    0x6C,
    0xFE,
    0xFE,
    0x7C,
    0x38,
    0x10,
};
static uint32_t shoot_timer_ms = 0;
void sub_game_boss_update()
{
    if (!boss.active)
        return;

    /* Di chuyển lên xuống - time-based */
    boss.y_fp += (int16_t)(boss.dir * BOSS_SPEED_PPS * g_game_clock.delta_ms) / 1000 << FP_SHIFT;
    boss.y = boss.y_fp >> FP_SHIFT;

    if (boss.y >= SUBMARINE_Y_MAX - BOSS_HEIGHT)
    {
        boss.dir = -1;
        boss.y = SUBMARINE_Y_MAX - BOSS_HEIGHT;
        boss.y_fp = boss.y << FP_SHIFT;
    }
    if (boss.y <= SUBMARINE_Y_MIN)
    {
        boss.dir = 1;
        boss.y = SUBMARINE_Y_MIN;
        boss.y_fp = boss.y << FP_SHIFT;
    }

    /* Update đạn boss - time-based */
    int16_t bullet_dx = ((int16_t)(BOSS_BULLET_SPEED_PPS * g_game_clock.delta_ms) / 1000) << FP_SHIFT;
    for (uint8_t i = 0; i < BOSS_BULLET_MAX; i++)
    {
        if (!boss_bullets[i].active)
            continue;
        boss_bullets[i].x_fp -= bullet_dx;
        boss_bullets[i].x = boss_bullets[i].x_fp >> FP_SHIFT;
        if (boss_bullets[i].x < 0)
        {
            boss_bullets[i].active = 0;
        }
    }

    /* Boss bắn - time-based */
    shoot_timer_ms += g_game_clock.delta_ms;
    if (shoot_timer_ms >= BOSS_SHOOT_INTERVAL_MS)
    {
        shoot_timer_ms = 0;
        uint8_t slot = 0;
        for (uint8_t i = 0; i < BOSS_BULLET_MAX && slot < 2; i++)
        {
            if (boss_bullets[i].active)
                continue;
            boss_bullets[i].active = 1;
            boss_bullets[i].x = boss.x;
            boss_bullets[i].x_fp = boss.x << FP_SHIFT;
            boss_bullets[i].y = (slot == 0) ? boss.y + 4 : boss.y + BOSS_HEIGHT - 6;
            slot++;
        }
    }
}

void sub_game_boss_setup()
{
    boss.x = LCD_WIDTH - BOSS_WIDTH - 2;
    boss.y = (LCD_HEIGHT - BOSS_HEIGHT) / 2;
    boss.y_fp = boss.y << FP_SHIFT;
    boss.dir = 1;
    boss.active = 0;
    shoot_timer_ms = 0;

    /* HP theo difficulty */
    switch (game_settings.speed)
    {
    case SPEED_EASY:
        boss.hp = BOSS_HP_EASY;
        break;
    case SPEED_HARD:
        boss.hp = BOSS_HP_HARD;
        break;
    default:
        boss.hp = BOSS_HP_NORMAL;
        break;
    }

    for (uint8_t i = 0; i < BOSS_BULLET_MAX; i++)
    {
        boss_bullets[i].active = 0;
        boss_bullets[i].x_fp = 0;
    }
}
void sub_game_boss_draw()
{
    if (!boss.active)
        return;

    view_render.drawBitmap(boss.x, boss.y,
                           boss_bitmap,
                           BOSS_WIDTH, BOSS_HEIGHT, WHITE);

    /* Vẽ đạn boss */
    for (uint8_t i = 0; i < BOSS_BULLET_MAX; i++)
    {
        if (!boss_bullets[i].active)
            continue;
        view_render.fillRect(boss_bullets[i].x,
                             boss_bullets[i].y,
                             5, 2, WHITE);
    }
}

void sub_game_boss_draw_hp()
{
    if (!boss.active)
        return;

    /* Vẽ thanh HP boss */
  uint8_t boss_hp_max;
switch (game_settings.speed) {
    case SPEED_EASY:  boss_hp_max = BOSS_HP_EASY;   break;
    case SPEED_HARD:  boss_hp_max = BOSS_HP_HARD;   break;
    default:          boss_hp_max = BOSS_HP_NORMAL; break;
}
uint8_t bar_width = (boss.hp * 60) / boss_hp_max;
    view_render.drawRect(34, 2, 60, 5, WHITE);
    if (bar_width > 0)
    {
        view_render.fillRect(34, 2, bar_width, 5, WHITE);
    }

    /* Chữ BOSS */
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(96, 2);
    view_render.print("BOSS");
}

uint8_t sub_game_boss_hit_by_torpedo()
{
    if (!boss.active)
        return 0;

    for (uint8_t i = 0; i < TORPEDO_MAX; i++)
    {
        if (!torpedoes[i].active)
            continue;
        int16_t torpedo_x = torpedoes[i].x_fp >> FP_SHIFT;
        if (torpedo_x < boss.x + BOSS_WIDTH &&
            torpedo_x + TORPEDO_WIDTH > boss.x &&
            torpedoes[i].y < boss.y + BOSS_HEIGHT &&
            torpedoes[i].y + TORPEDO_HEIGHT > boss.y)
        {
            torpedoes[i].active = 0;
            boss.hp -= 10;
            if (boss.hp <= 0)
            {
                boss.hp = 0;
                boss.active = 0;
            }
            return 1;
        }
    }
    return 0;
}

uint8_t sub_game_boss_bullet_hit_submarine()
{
    if (!boss.active)
        return 0;

    for (uint8_t i = 0; i < BOSS_BULLET_MAX; i++)
    {
        if (!boss_bullets[i].active)
            continue;
        if (boss_bullets[i].x < submarine.x + SUBMARINE_WIDTH &&
            boss_bullets[i].x + 5 > submarine.x &&
            boss_bullets[i].y < submarine.y + SUBMARINE_HEIGHT &&
            boss_bullets[i].y + 2 > submarine.y)
        {
            boss_bullets[i].active = 0;
            return 1;
        }
    }
    return 0;
}

void sub_game_boss_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SB_GAME_BOSS_SETUP:
    {
        sub_game_boss_setup();
    }
    break;

    case SB_GAME_BOSS_UPDATE:
    {
        sub_game_boss_update();
    }
    break;

    default:
        break;
    }
}