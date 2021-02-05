//
// Created by bobby on 02/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_MAX_MOD_SFX_HPP
#define MORPHEUS_GBA_TEST_MAX_MOD_SFX_HPP

#ifdef _GBA
#include <maxmod.h>
#elif _NDS
#include <maxmod9.h>
#endif

namespace morpheus {
    namespace core {
        namespace audio {
            class MaxModSfx {
            public:
                MaxModSfx(unsigned int sound_bank_ref_num) {
                    m_sound_bank_ref_num = sound_bank_ref_num;
                }

                unsigned char get_panning() const {
                    return m_sfx_panning;
                }

                void set_panning(const unsigned char panning) {
                    m_sfx_panning = panning;

                    if(m_sfx_handle >= 0) {
                        mmEffectPanning(m_sfx_handle, panning);
                    }
                }

                unsigned short get_rate() const {
                    return m_sfx_rate;
                }

                void set_rate(const unsigned short rate) {
                    m_sfx_rate = rate;

                    if(m_sfx_handle >= 0) {
                        mmEffectRate(m_sfx_handle, m_sfx_rate);
                    }
                }

                unsigned char get_volume() const {
                    return m_sfx_volume;
                }

                void set_volume(const unsigned char volume) {
                    m_sfx_volume = volume;

                    if(m_sfx_handle >= 0) {
                        mmEffectVolume(m_sfx_handle, m_sfx_volume);
                    }
                }

                void stop_effect() {
                    mmEffectCancel(m_sfx_handle);
                }

                void start_effect(bool low_priority);
            protected:
                unsigned int get_sound_bank_ref_num() {
                    return m_sound_bank_ref_num;
                }

                virtual void load_effect() = 0;
            private:
                bool m_loaded = false;
                unsigned short m_sfx_handle = -1;
                unsigned char m_sfx_panning = 128;
                unsigned short m_sfx_rate = 1024;
                unsigned char m_sfx_volume = 255;
                unsigned int m_sound_bank_ref_num;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_MAX_MOD_SFX_HPP
