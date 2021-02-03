//
// Created by bobby on 02/02/2021.
//

#include <core/audio/max_mod_music.hpp>

void morpheus::core::audio::MaxModMusic::play_music(bool looped) {
    if(!m_loaded) {
        load_music();

        m_loaded = true;
    }

    mmStart(m_sound_bank_ref_num, looped ? MM_PLAY_LOOP : MM_PLAY_ONCE);
}
