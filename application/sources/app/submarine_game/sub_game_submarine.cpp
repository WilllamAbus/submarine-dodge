#include "sub_game_submarine.h"
#include "game_time.h"
#define SUBMARINE_AUTO_SPEED_PPS  8
/* Bitmap tàu ngầm 16x8 */
static const uint8_t submarine_bitmap[] = {
    0x00, 0x00,
    0x0C, 0x00,
    0x1F, 0xF0,
    0x7F, 0xFE,
    0xFF, 0xFF,
    0x7F, 0xFE,
    0x3F, 0xFC,
    0x00, 0x00};

/* Bitmap trái tim 7x6px */
static const uint8_t heart_bitmap[] = {
    0x6C, /* .XX.XX. */
    0xFE, /* XXXXXXX */
    0xFE, /* XXXXXXX */
    0x7C, /* .XXXXX. */
    0x38, /* ..XXX.. */
    0x10, /* ...X... */
};
   /* pixel/giây, di chuyển nhẹ */


/* Biến toàn cục */
submarine_t submarine;
static int8_t  auto_move_dir = 1;
static int16_t auto_y_fp     = 0;
uint8_t submarine_hp = SUBMARINE_HP_MAX;


void sub_game_submarine_draw_hp()
{
    for (uint8_t i = 0; i < submarine_hp; i++)
    {
        view_render.drawBitmap(2 + i * 10, 2, heart_bitmap, 7, 6, WHITE);
    }
}
void sub_game_submarine_setup() {
    submarine.x  = SUBMARINE_X;
    submarine.y  = (SUBMARINE_Y_MAX - SUBMARINE_Y_MIN) / 2;
    submarine.is_alive = 1;
    submarine_hp = SUBMARINE_HP_MAX;
    auto_move_dir = 1;
    auto_y_fp = submarine.y << FP_SHIFT;
}
void sub_game_submarine_up()
{
    if (submarine.y - SUBMARINE_STEP >= SUBMARINE_Y_MIN)
    {
        submarine.y -= SUBMARINE_STEP;
    }
}

void sub_game_submarine_down()
{
    if (submarine.y + SUBMARINE_STEP <= SUBMARINE_Y_MAX - SUBMARINE_HEIGHT)
    {
        submarine.y += SUBMARINE_STEP;
    }
}

void sub_game_submarine_reset()
{
    sub_game_submarine_setup();
}

void sub_game_submarine_draw()
{
    view_render.drawBitmap(
        submarine.x,
        submarine.y,
        submarine_bitmap,
        SUBMARINE_WIDTH,
        SUBMARINE_HEIGHT,
        WHITE);
}

void sub_game_submarine_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SB_GAME_SUBMARINE_SETUP:
    {
        APP_DBG_SIG("SB_GAME_SUBMARINE_SETUP\n");
        sub_game_submarine_setup();
    }
    break;

    case SB_GAME_SUBMARINE_UP:
    {
        APP_DBG_SIG("SB_GAME_SUBMARINE_UP\n");
        sub_game_submarine_up();
    }
    break;

    case SB_GAME_SUBMARINE_DOWN:
    {
        APP_DBG_SIG("SB_GAME_SUBMARINE_DOWN\n");
        sub_game_submarine_down();
    }
    break;
case SB_GAME_SUBMARINE_UPDATE: {
    /* Di chuyển tự động nhẹ - time-based */
    /* delta_ms tính bằng ms, speed = pixel/giây */
    int16_t dy = (int16_t)(auto_move_dir * SUBMARINE_AUTO_SPEED_PPS * g_game_clock.delta_ms) / 1000;
    auto_y_fp += dy << FP_SHIFT;
    submarine.y = auto_y_fp >> FP_SHIFT;

    if (submarine.y >= SUBMARINE_Y_MAX - SUBMARINE_HEIGHT - 2) {
        auto_move_dir = -1;
        submarine.y   = SUBMARINE_Y_MAX - SUBMARINE_HEIGHT - 2;
        auto_y_fp     = submarine.y << FP_SHIFT;
    }
    if (submarine.y <= SUBMARINE_Y_MIN + 2) {
        auto_move_dir = 1;
        submarine.y   = SUBMARINE_Y_MIN + 2;
        auto_y_fp     = submarine.y << FP_SHIFT;
    }
}
break;
    default:
        break;
    }
}