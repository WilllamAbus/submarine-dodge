#include "sub_game_submarine.h"

/* Bitmap tàu ngầm 16x8 */
static const uint8_t submarine_bitmap[] = {
    0x00, 0x00,
    0x0C, 0x00,
    0x1F, 0xF0,
    0x7F, 0xFE,
    0xFF, 0xFF,
    0x7F, 0xFE,
    0x3F, 0xFC,
    0x00, 0x00
};

/* Biến toàn cục */
submarine_t submarine;

void sub_game_submarine_setup() {
    submarine.x = SUBMARINE_X;
    submarine.y = (SUBMARINE_Y_MAX - SUBMARINE_Y_MIN) / 2;
    submarine.is_alive = 1;
}

void sub_game_submarine_up() {
    if (submarine.y - SUBMARINE_STEP >= SUBMARINE_Y_MIN) {
        submarine.y -= SUBMARINE_STEP;
    }
}

void sub_game_submarine_down() {
    if (submarine.y + SUBMARINE_STEP <= SUBMARINE_Y_MAX - SUBMARINE_HEIGHT) {
        submarine.y += SUBMARINE_STEP;
    }
}

void sub_game_submarine_reset() {
    sub_game_submarine_setup();
}

void sub_game_submarine_draw() {
    view_render.drawBitmap(
        submarine.x,
        submarine.y,
        submarine_bitmap,
        SUBMARINE_WIDTH,
        SUBMARINE_HEIGHT,
        WHITE
    );
}


void sub_game_submarine_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case SB_GAME_SUBMARINE_SETUP: {
        APP_DBG_SIG("SB_GAME_SUBMARINE_SETUP\n");
        sub_game_submarine_setup();
    }
    break;

    case SB_GAME_SUBMARINE_UP: {
        APP_DBG_SIG("SB_GAME_SUBMARINE_UP\n");
        sub_game_submarine_up();
    }
    break;

    case SB_GAME_SUBMARINE_DOWN: {
        APP_DBG_SIG("SB_GAME_SUBMARINE_DOWN\n");
        sub_game_submarine_down();
    }
    break;

    default:
        break;
    }
}