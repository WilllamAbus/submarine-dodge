#include "game_scheduler.h"
#include "sub_game_boss.h"

game_phase_t g_game_phase;
game_scheduler_t g_scheduler;

void game_scheduler_init()
{
    g_game_phase = GAME_PHASE_WAVE;

    g_scheduler.phase = GAME_PHASE_WAVE;
    g_scheduler.boss_index = 0;
}

void game_scheduler_update(uint32_t score)
{
    switch (g_game_phase)
    {
    case GAME_PHASE_WAVE:

        if (score >= BOSS_SCORE_TRIGGER)
        {
            boss.active = 1;
            g_game_phase = GAME_PHASE_BOSS;
            g_scheduler.phase = GAME_PHASE_BOSS;
        }
        break;

    case GAME_PHASE_BOSS:

        if (boss.hp == 0)
        {
            g_game_phase = GAME_PHASE_VICTORY;
            g_scheduler.phase = GAME_PHASE_VICTORY;
        }
        break;

    default:
        break;
    }
}

void game_scheduler_boss_killed()
{
    g_game_phase = GAME_PHASE_VICTORY;
    g_scheduler.phase = GAME_PHASE_VICTORY;
}