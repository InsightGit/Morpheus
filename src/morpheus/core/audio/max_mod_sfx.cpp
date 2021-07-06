//
// Created by bobby on 04/02/2021.
//

#include "morpheus/core/audio/max_mod_sfx.hpp"

void morpheus::core::audio::MaxModSfx::start_effect(bool low_priority) {
    mm_sound_effect sfx;

    if(!m_loaded) {
        load_effect();

        m_loaded = true;
    }

    sfx.handle = 0;
    sfx.id = m_sound_bank_ref_num;
    sfx.panning = m_sfx_panning;
    sfx.rate = m_sfx_rate;
    sfx.volume = m_sfx_volume;

    m_sfx_handle = mmEffectEx(&sfx);

    m_started = true;

    if(low_priority) {
        mmEffectRelease(m_sfx_handle);
    }
}
