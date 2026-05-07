#include "scr_main_menu.h"
#include "scr_submarine_game.h"

/* ==================== MENU DATA ==================== */
static const char* menu_items_name[NUMBER_MENU_ITEMS] = {
    " Submarine Dodge",
    " Brick Breaker  ",
};

static uint8_t menu_selected = 0;  /* Item đang chọn */

/* ==================== VIEW ==================== */
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
    view_render.setCursor(20, 2);
    view_render.print("== GAME MENU ==");
    view_render.drawFastHLine(0, 12, LCD_WIDTH, WHITE);

    /* Danh sách game */
    for (uint8_t i = 0; i < NUMBER_MENU_ITEMS; i++) {
        uint8_t y = 18 + i * 18;

        if (i == menu_selected) {
            /* Item đang chọn: vẽ nền trắng */
            view_render.fillRoundRect(2, y - 2, LCD_WIDTH - 4, 14, 2, WHITE);
            view_render.setTextColor(BLACK);
        } else {
            view_render.drawRoundRect(2, y - 2, LCD_WIDTH - 4, 14, 2, WHITE);
            view_render.setTextColor(WHITE);
        }

        view_render.setCursor(8, y);
        view_render.print(menu_items_name[i]);
    }

    /* Hướng dẫn nút */
    view_render.drawFastHLine(0, 54, LCD_WIDTH, WHITE);
    view_render.setTextColor(WHITE);
    view_render.setTextSize(1);
    view_render.setCursor(2, 56);
    view_render.print("UP/DN:chon MODE:vao");
}

/* ==================== HANDLER ==================== */
static void screen_tran_menu() {
    switch (menu_selected) {
    case 0:
        SCREEN_TRAN(scr_submarine_game_handle, &scr_submarine_game);
        break;
    case 1:
        /* Brick Breaker - làm sau */
        SCREEN_TRAN(scr_submarine_game_handle, &scr_submarine_game);
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
        APP_DBG_SIG("AC_DISPLAY_BUTON_UP_RELEASED\n");
        if (menu_selected > 0) {
            menu_selected--;
        }
        BUZZER_PlayTones(tones_3beep);
    }
    break;

    case AC_DISPLAY_BUTON_DOWN_RELEASED: {
        APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_RELEASED\n");
        if (menu_selected < NUMBER_MENU_ITEMS - 1) {
            menu_selected++;
        }
        BUZZER_PlayTones(tones_3beep);
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED: {
        APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_RELEASED\n");
        screen_tran_menu();
    }
    break;

    default:
        break;
    }
}