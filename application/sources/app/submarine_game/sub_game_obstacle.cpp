#include "sub_game_obstacle.h"
#include "sub_game_boss.h"
#include "scr_settings.h"
obstacle_t obstacles[OBSTACLE_MAX];

/* Bitmap chướng ngại vật 8x8 (hình mìn) */
static const uint8_t obstacle_bitmap[] = {
    0x00,
    0x00, /* ........ ........ */
    0x00,
    0x18,
    /* ........ ...XX... */ /* ống kính */
    0x0F,
    0xF8,
    /* ....XXXX XXXXX... */ /* thân trên */
    0x3F,
    0xFE,
    /* ..XXXXXX XXXXXXX. */ /* thân giữa */
    0xFF,
    0xFF,
    /* XXXXXXXX XXXXXXXX */ /* thân chính */
    0x3F,
    0xFE,
    /* ..XXXXXX XXXXXXX. */ /* cánh */
    0x0F,
    0xF8,
    /* ....XXXX XXXXX... */ /* đuôi */
    0x00,
    0x00, /* ........ ........ */
};

void sub_game_obstacle_setup()
{
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
    {
        obstacles[i].active = 0;
    }
}

void sub_game_obstacle_spawn(uint8_t from_right)
{
    /* Tìm slot trống */
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
    {
        if (obstacles[i].active)
            continue;

        obstacles[i].active = 1;
        obstacles[i].y = SUBMARINE_Y_MIN +
                         (rand() % (SUBMARINE_Y_MAX - SUBMARINE_Y_MIN - OBSTACLE_HEIGHT));

        if (from_right)
        {
            obstacles[i].x = LCD_WIDTH;
            obstacles[i].dir = -1; /* Bay từ phải sang trái */
        }
        else
        {
            obstacles[i].x = -OBSTACLE_WIDTH;
            obstacles[i].dir = +1; /* Bay từ trái sang phải */
        }
        break;
    }
}

void sub_game_obstacle_update() {
    uint8_t speed;
    switch (game_settings.speed) {
        case SPEED_EASY:  speed = OBSTACLE_SPEED_EASY;   break;
        case SPEED_HARD:  speed = OBSTACLE_SPEED_HARD;   break;
        default:          speed = OBSTACLE_SPEED_NORMAL; break;
    }

    for (uint8_t i = 0; i < OBSTACLE_MAX; i++) {
        if (!obstacles[i].active) continue;
        obstacles[i].x += obstacles[i].dir * speed;
        if (obstacles[i].x > LCD_WIDTH ||
            obstacles[i].x < -OBSTACLE_WIDTH) {
            obstacles[i].active = 0;
        }
    }
}
void sub_game_obstacle_draw()
{
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
    {
        if (!obstacles[i].active)
            continue;
        view_render.drawBitmap(
            obstacles[i].x,
            obstacles[i].y,
            obstacle_bitmap,
            OBSTACLE_WIDTH,
            OBSTACLE_HEIGHT,
            WHITE);
    }
}

/* Kiểm tra obstacle có va chạm tàu ngầm không */
uint8_t sub_game_obstacle_hit_submarine()
{
    for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
    {
        if (!obstacles[i].active)
            continue;

        if (obstacles[i].x < submarine.x + SUBMARINE_WIDTH &&
            obstacles[i].x + OBSTACLE_WIDTH > submarine.x &&
            obstacles[i].y < submarine.y + SUBMARINE_HEIGHT &&
            obstacles[i].y + OBSTACLE_HEIGHT > submarine.y)
        {
            obstacles[i].active = 0;
            return 1;
        }
    }
    return 0;
}

enemy_bullet_t enemy_bullets[ENEMY_BULLET_MAX];

void sub_game_obstacle_draw_bullets()
{
    for (uint8_t i = 0; i < ENEMY_BULLET_MAX; i++)
    {
        if (!enemy_bullets[i].active)
            continue;
        view_render.fillRect(enemy_bullets[i].x, enemy_bullets[i].y, 4, 2, WHITE);
    }
}

uint8_t sub_game_enemy_bullet_hit_submarine()
{
    for (uint8_t i = 0; i < ENEMY_BULLET_MAX; i++)
    {
        if (!enemy_bullets[i].active)
            continue;
        if (enemy_bullets[i].x < submarine.x + SUBMARINE_WIDTH &&
            enemy_bullets[i].x + 4 > submarine.x &&
            enemy_bullets[i].y < submarine.y + SUBMARINE_HEIGHT &&
            enemy_bullets[i].y + 2 > submarine.y)
        {
            enemy_bullets[i].active = 0;
            return 1;
        }
    }
    return 0;
}

void sub_game_obstacle_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
    case SB_GAME_OBSTACLE_SETUP:
    {
        sub_game_obstacle_setup();
        for (uint8_t i = 0; i < ENEMY_BULLET_MAX; i++)
        {
            enemy_bullets[i].active = 0;
        }
    }
    break;

    case SB_GAME_OBSTACLE_RUN:
    {
        sub_game_obstacle_update();

        /* Update đạn địch */
        for (uint8_t i = 0; i < ENEMY_BULLET_MAX; i++)
        {
            if (!enemy_bullets[i].active)
                continue;
            enemy_bullets[i].x -= ENEMY_BULLET_SPEED;
            if (enemy_bullets[i].x < 0)
            {
                enemy_bullets[i].active = 0;
            }
        }

        /* Chỉ spawn và bắn khi boss chưa xuất hiện */
        if (!boss.active)
        {
            /* Tàu địch bắn tự động */
            static uint8_t shoot_tick = 0;
            shoot_tick++;
            if (shoot_tick >= 8)
            {
                shoot_tick = 0;
                for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
                {
                    if (!obstacles[i].active)
                        continue;
                    for (uint8_t j = 0; j < ENEMY_BULLET_MAX; j++)
                    {
                        if (enemy_bullets[j].active)
                            continue;
                        enemy_bullets[j].active = 1;
                        enemy_bullets[j].x = obstacles[i].x;
                        enemy_bullets[j].y = obstacles[i].y + OBSTACLE_HEIGHT / 2;
                        break;
                    }
                }
            }

            /* Spawn obstacle */
            /* Spawn obstacle theo speed */
            static uint8_t spawn_tick = 0;
            spawn_tick++;

            uint8_t spawn_interval;
            switch (game_settings.speed)
            {
            case SPEED_EASY:
                spawn_interval = 8;
                break;
            case SPEED_HARD:
                spawn_interval = 3;
                break;
            default:
                spawn_interval = 5;
                break;
            }

            if (spawn_tick >= spawn_interval)
            {
                spawn_tick = 0;
                sub_game_obstacle_spawn(1);
            }
        }
        else
        {
            /* Boss xuất hiện → xóa hết obstacle và đạn còn lại */
            for (uint8_t i = 0; i < OBSTACLE_MAX; i++)
            {
                obstacles[i].active = 0;
            }
            for (uint8_t i = 0; i < ENEMY_BULLET_MAX; i++)
            {
                enemy_bullets[i].active = 0;
            }
        }
    }
    break;

    case SB_GAME_OBSTACLE_RESET:
    {
        sub_game_obstacle_setup();
    }
    break;

    default:
        break;
    }
}
