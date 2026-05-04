#include "sub_game_obstacle.h"

obstacle_t obstacles[OBSTACLE_MAX];

/* Bitmap chướng ngại vật 8x8 (hình mìn) */
static const uint8_t obstacle_bitmap[] = {
    0x18, 0x3C, 0x7E, 0xFF,
    0xFF, 0x7E, 0x3C, 0x18
};

void sub_game_obstacle_setup() {
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++) {
        obstacles[i].active = 0;
    }
}

void sub_game_obstacle_spawn(uint8_t from_right) {
    /* Tìm slot trống */
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++) {
        if (obstacles[i].active) continue;

        obstacles[i].active = 1;
        obstacles[i].y = SUBMARINE_Y_MIN + 
                         (rand() % (SUBMARINE_Y_MAX - SUBMARINE_Y_MIN - OBSTACLE_HEIGHT));

        if (from_right) {
            obstacles[i].x   = LCD_WIDTH;
            obstacles[i].dir  = -1;  /* Bay từ phải sang trái */
        } else {
            obstacles[i].x   = -OBSTACLE_WIDTH;
            obstacles[i].dir  = +1;  /* Bay từ trái sang phải */
        }
        break;
    }
}

void sub_game_obstacle_update() {
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++) {
        if (!obstacles[i].active) continue;

        obstacles[i].x += obstacles[i].dir * OBSTACLE_SPEED;

        /* Ra khỏi màn hình thì deactivate */
        if (obstacles[i].x > LCD_WIDTH || 
            obstacles[i].x < -OBSTACLE_WIDTH) {
            obstacles[i].active = 0;
        }
    }
}

void sub_game_obstacle_draw() {
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++) {
        if (!obstacles[i].active) continue;
        view_render.drawBitmap(
            obstacles[i].x,
            obstacles[i].y,
            obstacle_bitmap,
            OBSTACLE_WIDTH,
            OBSTACLE_HEIGHT,
            WHITE
        );
    }
}

/* Kiểm tra obstacle có va chạm tàu ngầm không */
uint8_t sub_game_obstacle_hit_submarine() {
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++) {
        if (!obstacles[i].active) continue;

        if (obstacles[i].x < submarine.x + SUBMARINE_WIDTH &&
            obstacles[i].x + OBSTACLE_WIDTH > submarine.x &&
            obstacles[i].y < submarine.y + SUBMARINE_HEIGHT &&
            obstacles[i].y + OBSTACLE_HEIGHT > submarine.y) {
            obstacles[i].active = 0;
            return 1;
        }
    }
    return 0;
}
