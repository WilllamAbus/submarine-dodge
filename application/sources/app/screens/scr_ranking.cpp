#include "scr_ranking.h"
#include "scr_main_menu.h"

ranking_t ranking;

void ranking_load() {
    eeprom_read(EEPROM_RANKING_ADDR,
                (uint8_t*)&ranking,
                sizeof(ranking_t));

    /* Nếu EEPROM chưa có dữ liệu thì reset */
    for (uint8_t i = 0; i < RANKING_MAX; i++) {
        if (ranking.scores[i] > 99999) {
            ranking.scores[i] = 0;
        }
    }
}

void ranking_save() {
    eeprom_write(EEPROM_RANKING_ADDR,
                 (uint8_t*)&ranking,
                 sizeof(ranking_t));
}

void ranking_update(uint32_t new_score) {
    ranking_load();

    /* Thêm điểm mới vào rồi sắp xếp giảm dần */
    for (uint8_t i = 0; i < RANKING_MAX; i++) {
        if (new_score > ranking.scores[i]) {
            /* Đẩy các điểm thấp hơn xuống */
            for (uint8_t j = RANKING_MAX - 1; j > i; j--) {
                ranking.scores[j] = ranking.scores[j - 1];
            }
            ranking.scores[i] = new_score;
            break;
        }
    }

    ranking_save();
}

/* ==================== VIEW ==================== */
static void view_scr_ranking();

view_dynamic_t dyn_view_ranking = {
    { .item_type = ITEM_TYPE_DYNAMIC },
    view_scr_ranking
};

view_screen_t scr_ranking = {
    &dyn_view_ranking,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

static void view_scr_ranking() {
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);

    /* Title */
    view_render.setCursor(38, 2);
    view_render.print("RANKING");
    view_render.drawFastHLine(0, 12, LCD_WIDTH, WHITE);

    /* Top 3 */
    const char* medals[] = {"Top 1", "Top 2", "Top 3"};
    for (uint8_t i = 0; i < RANKING_MAX; i++) {
        view_render.setCursor(10, 18 + i * 12);
        view_render.print(medals[i]);
        view_render.setCursor(55, 18 + i * 12);
        view_render.print(": ");
        view_render.print(ranking.scores[i]);
    }

    /* MODE back */
    view_render.drawFastHLine(0, 54, LCD_WIDTH, WHITE);
    view_render.setCursor(34, 56);
    view_render.print("MODE: back");
}

/* ==================== HANDLER ==================== */
void scr_ranking_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    case SCREEN_ENTRY: {
        APP_DBG_SIG("SCREEN_ENTRY\n");
        ranking_load();
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