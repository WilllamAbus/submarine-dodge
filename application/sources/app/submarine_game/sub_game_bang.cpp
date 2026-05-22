#include "sub_game_bang.h"

bang_t bangs[BANG_MAX];

/* Bitmap vụ nổ 16x16 */
static const uint8_t bang_bitmap[] = {
    0x08, 0x00,  /* ....X... ........ */
    0x49, 0x00,  /* .X..X..X ........ */
    0x2A, 0x80,  /* ..X.X.X. X....... */
    0x1C, 0x40,  /* ...XXX.. .X...... */
    0xFF, 0xE0,  /* XXXXXXXX XXX..... */
    0x1C, 0x40,  /* ...XXX.. .X...... */
    0x2A, 0x80,  /* ..X.X.X. X....... */
    0x49, 0x00,  /* .X..X..X ........ */
    0x08, 0x00,  /* ....X... ........ */
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

void sub_game_bang_setup() {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        bangs[i].active = 0;
        bangs[i].timer  = 0;
    }
}

void sub_game_bang_spawn(int8_t x, int8_t y) {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        if (bangs[i].active) continue;
        bangs[i].active = 1;
        bangs[i].timer  = BANG_DURATION;
        bangs[i].x      = x;
        bangs[i].y      = y;
        break;
    }
}

void sub_game_bang_update() {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        if (!bangs[i].active) continue;
        bangs[i].timer--;
        if (bangs[i].timer == 0) {
            bangs[i].active = 0;
        }
    }
}

void sub_game_bang_draw() {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        if (!bangs[i].active) continue;
        view_render.drawBitmap(
            bangs[i].x,
            bangs[i].y,
            bang_bitmap,
            BANG_WIDTH,
            BANG_HEIGHT,
            WHITE
        );
    }
}


void sub_game_bang_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case SB_GAME_BANG_SETUP: {
        sub_game_bang_setup();
    }
    break;

    case SB_GAME_BANG_UPDATE: {
        sub_game_bang_update();
    }
    break;

    case SB_GAME_BANG_RESET: {
        sub_game_bang_setup();
    }
    break;

    default:
        break;
    }
}