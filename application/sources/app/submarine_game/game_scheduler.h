#ifndef __GAME_SCHEDULER_H__
#define __GAME_SCHEDULER_H__

#include <stdint.h>

typedef enum
{
    GAME_PHASE_WAVE = 0,
    GAME_PHASE_BOSS,
    GAME_PHASE_VICTORY
} game_phase_t;

typedef struct
{
    game_phase_t phase;
    uint8_t boss_index;
} game_scheduler_t;


extern game_phase_t g_game_phase;
extern game_scheduler_t g_scheduler;

void game_scheduler_init();
void game_scheduler_update(uint32_t score);
void game_scheduler_boss_killed();

#endif