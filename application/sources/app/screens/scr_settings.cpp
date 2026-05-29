#include "scr_settings.h"
#include "scr_main_menu.h"
#include "scr_ranking.h"
#include "sub_game_obstacle.h"

settings_t game_settings;
static uint8_t settings_selected = 0;
static uint8_t confirm_reset = 0;

void settings_load()
{
    eeprom_read(EEPROM_SETTINGS_ADDR,
                (uint8_t *)&game_settings,
                sizeof(settings_t));

    /* Giá trị mặc định nếu EEPROM chưa có */
    if (game_settings.sound_on > 1)
    {
        game_settings.sound_on = 1;
    }
    if (game_settings.speed > SPEED_HARD)
    {
        game_settings.speed = SPEED_NORMAL;
    }
}

void settings_save()
{
    eeprom_write(EEPROM_SETTINGS_ADDR,
                 (uint8_t *)&game_settings,
                 sizeof(settings_t));
}

void settings_apply_speed()
{
    switch (game_settings.speed)
    {
    case SPEED_EASY:
        /* Obstacle chậm, spawn thưa */
        break;
    case SPEED_NORMAL:
        /* Mặc định */
        break;
    case SPEED_HARD:
        /* Obstacle nhanh, spawn dày */
        break;
    }
}

/* ==================== VIEW ==================== */
static void view_scr_settings();

view_dynamic_t dyn_view_settings = {
    {.item_type = ITEM_TYPE_DYNAMIC},
    view_scr_settings};

view_screen_t scr_settings = {
    &dyn_view_settings,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_settings()
{
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

    /* Title */
    view_render.setCursor(32, 2);
    view_render.print("SETTINGS");
    view_render.drawFastHLine(0, 12, LCD_WIDTH, WHITE);

    /* Confirm reset */
    if (confirm_reset)
    {
        view_render.setCursor(15, 25);
        view_render.print("Reset scores?");
        view_render.setCursor(15, 38);
        view_render.print("UP:Yes DOWN:No");
        return;
    }

    /* Menu items */
    const char *items[] = {"Sound", "Speed", "Reset", "Back"};
    for (uint8_t i = 0; i < SETTINGS_MENU_MAX; i++)
    {
        uint8_t y = 16 + i * 12;

        /* Mũi tên chỉ */
        if (i == settings_selected)
        {
            view_render.setCursor(2, y);
            view_render.print(">");
        }

        view_render.setCursor(12, y);
        view_render.print(items[i]);
        view_render.setCursor(55, y);
        view_render.print(": ");

        if (i == 0)
        {
            view_render.print(game_settings.sound_on ? "ON" : "OFF");
        }
        else if (i == 1)
        {
            const char *speeds[] = {"Easy", "Normal", "Hard"};
            view_render.print(speeds[game_settings.speed]);
        }
        else
        {
            view_render.print("Scores");
        }
    }

    /* Hướng dẫn */
    view_render.drawFastHLine(0, 54, LCD_WIDTH, WHITE);
    view_render.setCursor(2, 56);
    view_render.print("MODE:edit UP/DN:chon");
}

/* ==================== HANDLER ==================== */
void scr_settings_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {

    case SCREEN_ENTRY:
    {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        settings_load();
        settings_selected = 0;
        confirm_reset = 0;
    }
    break;

    case AC_DISPLAY_BUTON_UP_RELEASED:
    {
        if (confirm_reset)
        {
            /* Xác nhận reset */
            ranking_t empty = {0, 0, 0};
            eeprom_write(EEPROM_RANKING_ADDR,
                         (uint8_t *)&empty,
                         sizeof(ranking_t));
            confirm_reset = 0;
            BUZZER_PlayTones(tones_3beep);
        }
        else
        {
            if (settings_selected > 0)
                settings_selected--;
        }
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED:
    {
        if (confirm_reset)
        {
            /* Hủy reset */
            confirm_reset = 0;
        }
        else
        {
            if (settings_selected < SETTINGS_MENU_MAX - 1)
            {
                settings_selected++;
            }
        }
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED:
    {
        if (confirm_reset)
        {
            confirm_reset = 0;
            break;
        }

        switch (settings_selected)
        {
        case 0:
            /* Toggle Sound */
            game_settings.sound_on = !game_settings.sound_on;
            settings_save();
            if (game_settings.sound_on)
            {
                BUZZER_PlayTones(tones_3beep);
            }
            break;
        case 1:
            /* Đổi Speed */
            game_settings.speed = (game_speed_t)((game_settings.speed + 1) % 3);
            settings_save();
            break;
        case 2:
            /* Reset Scores - hỏi xác nhận */
            confirm_reset = 1;
            break;
        case 3:
            /* Back về Main Menu */
            SCREEN_TRAN(scr_main_menu_handle, &scr_main_menu);
            break;
        default:
            /* Thoát về Main Menu */
            SCREEN_TRAN(scr_main_menu_handle, &scr_main_menu);
            break;
        }
    }
    break;

    default:
        break;
    }
}