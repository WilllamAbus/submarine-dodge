#include "sub_game_bang.h"

bang_t bangs[BANG_MAX];

/* Bitmap vụ nổ 16x16 */
static const uint8_t bang_bitmap[] = {
    0x01, 0x00, 0x42, 0x10,
    0x24, 0x24, 0x18, 0x18,
    0xFF, 0xFF, 0x18, 0x18,
    0x24, 0x24, 0x42, 0x10,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
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
