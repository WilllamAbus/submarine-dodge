#include "scr_victory.h"
#include "sub_game_common.h"
#include "scr_main_menu.h"
#include "scr_ranking.h"
uint32_t victory_score = 0;

static void view_scr_victory();

view_dynamic_t dyn_view_victory = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_victory
};

view_screen_t scr_victory = {
    &dyn_view_victory,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_victory() {
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

    /* Victory */
    view_render.setCursor(38, 10);
    view_render.print("VICTORY!");

    /* Checkmark */
    view_render.setCursor(58, 22);
    view_render.print(":)");

    /* Score */
    view_render.setCursor(30, 35);
    view_render.print("Score: ");
    view_render.print(victory_score);

    /* MODE to menu */
    view_render.setCursor(28, 50);
    view_render.print("MODE to menu");
}

void scr_victory_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    case SCREEN_ENTRY: {

        APP_DBG_SIG("SCREEN_ENTRY\n");
          
        game_buzzer_play(tones_SMB);
         ranking_update(victory_score);
    }
    break;

    case AC_DISPLAY_BUTON_MODE_RELEASED: {
        SCREEN_TRAN(scr_main_menu_handle, &scr_main_menu);
    }
    break;

    default:
        break;
    }
}