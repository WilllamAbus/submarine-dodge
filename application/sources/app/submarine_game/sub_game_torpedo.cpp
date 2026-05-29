#include "sub_game_torpedo.h"
#include "sub_game_common.h"

torpedo_t torpedoes[TORPEDO_MAX];

/* Bitmap ngư lôi 8x3 */
static const uint8_t torpedo_bitmap[] = {
    0x38, 0x7C, 0x38
};

void sub_game_torpedo_setup() {
    for (uint8_t i = 0; i < TORPEDO_MAX; i++) {
        torpedoes[i].active = 0;
    }
}

void sub_game_torpedo_update() {
    for (uint8_t i = 0; i < TORPEDO_MAX; i++) {
        if (!torpedoes[i].active) continue;

        torpedoes[i].x += torpedoes[i].dir * TORPEDO_SPEED;

        /* Hết màn hình thì deactivate */
        if (torpedoes[i].x > LCD_WIDTH || torpedoes[i].x < 0) {
            torpedoes[i].active = 0;
        }
    }
}

void sub_game_torpedo_draw() {
    for (uint8_t i = 0; i < TORPEDO_MAX; i++) {
        if (!torpedoes[i].active) continue;
        view_render.drawBitmap(
            torpedoes[i].x,
            torpedoes[i].y,
            torpedo_bitmap,
            TORPEDO_WIDTH,
            TORPEDO_HEIGHT,
            WHITE
        );
    }
}

/* Kiểm tra ngư lôi có trúng object tại (x,y,w,h) không */
uint8_t sub_game_torpedo_hit(int8_t x, int8_t y, uint8_t w, uint8_t h) {
    for (uint8_t i = 0; i < TORPEDO_MAX; i++) {
        if (!torpedoes[i].active) continue;

        if (torpedoes[i].x < x + w &&
            torpedoes[i].x + TORPEDO_WIDTH > x &&
            torpedoes[i].y < y + h &&
            torpedoes[i].y + TORPEDO_HEIGHT > y) {
            torpedoes[i].active = 0;
            return 1;
        }
    }
    return 0;
}


void sub_game_torpedo_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case SB_GAME_TORPEDO_SETUP: {
        APP_DBG_SIG("SB_GAME_TORPEDO_SETUP\n");
        sub_game_torpedo_setup();
    }
    break;

    case SB_GAME_TORPEDO_SHOOT: {
        APP_DBG_SIG("SB_GAME_TORPEDO_SHOOT\n");
        /* Bắn từ vị trí tàu ngầm */
        for (uint8_t i = 0; i < TORPEDO_MAX; i++) {
            if (torpedoes[i].active) continue;
            torpedoes[i].active = 1;
            torpedoes[i].x     = submarine.x + SUBMARINE_WIDTH;
            torpedoes[i].y     = submarine.y + SUBMARINE_HEIGHT / 2;
            torpedoes[i].dir   = +1;  /* Bay sang phải */
            game_buzzer_play(tones_3beep);     
            break;
        }
    }
    break;

    case SB_GAME_TORPEDO_RUN: {
        APP_DBG_SIG("SB_GAME_TORPEDO_RUN\n");
        sub_game_torpedo_update();
    }
    break;

    case SB_GAME_TORPEDO_RESET: {
        APP_DBG_SIG("SB_GAME_TORPEDO_RESET\n");
        sub_game_torpedo_setup();
    }
    break;

    default:
        break;
    }
}