// sub_game_common.h
#include "scr_settings.h"
#include "buzzer.h"

#ifndef __SUB_GAME_COMMON_H__
#define __SUB_GAME_COMMON_H__

inline void game_buzzer_play(const Tone_TypeDef* tones) {
    if (game_settings.sound_on) {
        BUZZER_PlayTones(tones);
    }
}

#endif