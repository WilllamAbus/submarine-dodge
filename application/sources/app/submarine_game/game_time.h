#pragma once

#include <stdint.h>
#define FP_SHIFT 8
#define FP_ONE   (1 << FP_SHIFT)
typedef struct
{
    uint32_t now_ms;
    uint16_t delta_ms;
} game_clock_t;

extern game_clock_t g_game_clock;

