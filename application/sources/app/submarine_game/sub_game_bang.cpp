#include "sub_game_bang.h"

bang_t bangs[BANG_MAX];

/* Frame 0 - điểm nhỏ 3x3 */
static const uint8_t bang_frame0[] = {
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x01, 0x80,  /* ......X. X....... */
    0x03, 0xC0,  /* .....XX. XX...... */
    0x01, 0x80,  /* ......X. X....... */
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

/* Frame 1 - nổ nhỏ */
static const uint8_t bang_frame1[] = {
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x04, 0x20,  /* .....X.. ..X..... */
    0x02, 0x40,  /* ......X. .X...... */
    0x1C, 0x70,  /* ...XXX.. .XXX.... */
    0x3E, 0xF8,  /* ..XXXXX. XXXXX... */
    0x1C, 0x70,  /* ...XXX.. .XXX.... */
    0x02, 0x40,
    0x04, 0x20,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

/* Frame 2 - nổ lớn */
static const uint8_t bang_frame2[] = {
    0x00, 0x00,
    0x08, 0x10,
    0x04, 0x20,
    0x42, 0x42,
    0x3E, 0xF8,
    0x7F, 0xFC,
    0xFF, 0xFE,
    0x7F, 0xFC,
    0x3E, 0xF8,
    0x42, 0x42,
    0x04, 0x20,
    0x08, 0x10,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

/* Frame 3 - tan ra */
static const uint8_t bang_frame3[] = {
    0x08, 0x10,
    0x00, 0x00,
    0x20, 0x04,
    0x00, 0x00,
    0x42, 0x42,
    0x00, 0x00,
    0x10, 0x08,
    0x00, 0x00,
    0x42, 0x42,
    0x00, 0x00,
    0x20, 0x04,
    0x00, 0x00,
    0x08, 0x10,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};
static const uint8_t* bang_frames[BANG_FRAME_COUNT] = {
    bang_frame0,
    bang_frame1,
    bang_frame2,
    bang_frame3,
};
void sub_game_bang_setup() {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        bangs[i].active   = 0;
        bangs[i].frame    = 0;
        bangs[i].timer_ms = 0;
    }
}

void sub_game_bang_spawn(int8_t x, int8_t y) {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        if (bangs[i].active) continue;
        bangs[i].active   = 1;
        bangs[i].frame    = 0;
        bangs[i].timer_ms = BANG_FRAME_MS;
        bangs[i].x        = x;
        bangs[i].y        = y;
        break;
    }
}


void sub_game_bang_update() {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        if (!bangs[i].active) continue;

        if (bangs[i].timer_ms <= g_game_clock.delta_ms) {
            bangs[i].frame++;
            if (bangs[i].frame >= BANG_FRAME_COUNT) {
                bangs[i].active = 0;
                bangs[i].frame  = 0;
            } else {
                bangs[i].timer_ms = BANG_FRAME_MS;
            }
        } else {
            bangs[i].timer_ms -= g_game_clock.delta_ms;
        }
    }
}

void sub_game_bang_update_all() {
    sub_game_bang_update();
}

void sub_game_bang_draw() {
    for (uint8_t i = 0; i < BANG_MAX; i++) {
        if (!bangs[i].active) continue;
        view_render.drawBitmap(
            bangs[i].x,
            bangs[i].y,
            bang_frames[bangs[i].frame],
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