#include "scr_main_menu.h"
#include "scr_submarine_game.h"
#include "scr_loading.h"
#include "scr_ranking.h"
#include "scr_settings.h"
#define NUMBER_MENU_ITEMS   (3)

static const char* menu_items_name[NUMBER_MENU_ITEMS] = {
    "Submarine Dodge",
    "Ranking",
    "Settings",
};


static uint8_t menu_selected = 0;

static void view_scr_main_menu();

view_dynamic_t dyn_view_main_menu = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_main_menu
};

view_screen_t scr_main_menu = {
    &dyn_view_main_menu,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_main_menu() {
    /* Title */
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(35, 2);
    view_render.print("GAME MENU");
    view_render.drawFastHLine(0, 12, LCD_WIDTH, WHITE);

    /* Danh sách game với mũi tên chỉ */
for (uint8_t i = 0; i < NUMBER_MENU_ITEMS; i++) {
    uint8_t y = 16 + i * 13;  /* Giảm từ 18 xuống 13 */

    if (i == menu_selected) {
        view_render.setCursor(2, y);
        view_render.print(">");
    }

    view_render.setCursor(12, y);
    view_render.setTextColor(WHITE);
    view_render.print(menu_items_name[i]);
}

    /* Hướng dẫn */
    view_render.drawFastHLine(0, 54, LCD_WIDTH, WHITE);
    view_render.setCursor(1, 56);
    view_render.setTextColor(WHITE);
    view_render.print("UP/DN:go | MODE:start");
}

static void screen_tran_menu() {
    switch (menu_selected) {
    case 0:
        SCREEN_TRAN(scr_loading_handle, &scr_loading);
        break;
    case 1:
        SCREEN_TRAN(scr_ranking_handle, &scr_ranking);
        break;
    case 2:
        SCREEN_TRAN(scr_settings_handle, &scr_settings);
    break;
    default:
        break;
    }
}

void scr_main_menu_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    case SCREEN_ENTRY: {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        menu_selected = 0;
    }
    break;

    case AC_DISPLAY_BUTON_UP_RELEASED: {
        if (menu_selected > 0) {
            menu_selected--;
        }
        BUZZER_PlayTones(tones_3beep);
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED: {
        if (menu_selected < NUMBER_MENU_ITEMS - 1) {
            menu_selected++;
        }
        BUZZER_PlayTones(tones_3beep);
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED: {
        screen_tran_menu();
    }
    break;

    default:
        break;
    }
}