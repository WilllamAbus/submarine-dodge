#include "sub_game_submarine.h"
#include "game_time.h"
#define SUB_ACCEL_PPS2      120
#define SUB_DRAG_PPS2        45
#define SUB_MAX_SPEED_PPS    55
submarine_t submarine;
static uint8_t thrust_up   = 0;
static uint8_t thrust_down = 0;
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

uint8_t submarine_hp = SUBMARINE_HP_MAX;


void sub_game_submarine_draw_hp()
{
    for (uint8_t i = 0; i < submarine_hp; i++)
    {
        view_render.drawBitmap(2 + i * 10, 2, heart_bitmap, 7, 6, WHITE);
    }
}
void sub_game_submarine_setup() {
 submarine.x = SUBMARINE_X;
submarine.y = (SUBMARINE_Y_MAX - SUBMARINE_Y_MIN) / 2;

submarine.y_fp = submarine.y << FP_SHIFT;
submarine.velocity_y_fp = 0;
thrust_up = 0;
thrust_down = 0;

submarine.is_alive = 1;
submarine_hp = SUBMARINE_HP_MAX;
}
void sub_game_submarine_up()
{
submarine.velocity_y_fp -=
    (18 * FP_ONE);

}

void sub_game_submarine_down()
{submarine.velocity_y_fp +=
    (18 * FP_ONE);
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
case SB_GAME_SUBMARINE_UPDATE:
{
    sub_game_submarine_update();
}
break;
    default:
        break;
    }
}

void sub_game_submarine_update()
{
    int32_t dt = g_game_clock.delta_ms;
int32_t accel =
    (SUB_ACCEL_PPS2 * FP_ONE * dt) / 1000;

if (thrust_up)
{
    submarine.velocity_y_fp -= accel;
}

if (thrust_down)
{
    submarine.velocity_y_fp += accel;
}
    int32_t drag =
        (SUB_DRAG_PPS2 * FP_ONE * dt) / 1000;

    if (submarine.velocity_y_fp > 0)
    {
        submarine.velocity_y_fp -= drag;

        if (submarine.velocity_y_fp < 0)
        {
            submarine.velocity_y_fp = 0;
        }
    }
    else if (submarine.velocity_y_fp < 0)
    {
        submarine.velocity_y_fp += drag;

        if (submarine.velocity_y_fp > 0)
        {
            submarine.velocity_y_fp = 0;
        }
    }

    int32_t max_speed =
        SUB_MAX_SPEED_PPS * FP_ONE;

    if (submarine.velocity_y_fp > max_speed)
    {
        submarine.velocity_y_fp = max_speed;
    }

    if (submarine.velocity_y_fp < -max_speed)
    {
        submarine.velocity_y_fp = -max_speed;
    }

    submarine.y_fp +=
        (submarine.velocity_y_fp * dt) / 1000;

    submarine.y =
        submarine.y_fp >> FP_SHIFT;

    if (submarine.y < SUBMARINE_Y_MIN)
    {
        submarine.y = SUBMARINE_Y_MIN;
        submarine.y_fp = submarine.y << FP_SHIFT;
        submarine.velocity_y_fp = 0;
    }

    if (submarine.y >
        SUBMARINE_Y_MAX - SUBMARINE_HEIGHT)
    {
        submarine.y =
            SUBMARINE_Y_MAX - SUBMARINE_HEIGHT;

        submarine.y_fp =
            submarine.y << FP_SHIFT;

        submarine.velocity_y_fp = 0;
    }
}