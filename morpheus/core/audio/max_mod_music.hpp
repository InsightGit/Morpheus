//
// Created by bobby on 02/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_MAX_MOD_MUSIC_HPP
#define MORPHEUS_GBA_TEST_MAX_MOD_MUSIC_HPP

#ifdef _GBA
#include <maxmod.h>
#elif _NDS
#include <maxmod9.h>
#endif

namespace morpheus {
    namespace core {
        namespace audio {
            class MaxModMusic {
            public:
                MaxModMusic(int sound_bank_ref_num) {
                    m_sound_bank_ref_num = sound_bank_ref_num;
                }

                virtual ~MaxModMusic() {
                    mmStop();
                }

                int get_music_pitch() const {
                    return m_pitch;
                }

                int get_music_tempo() const {
                    return m_tempo;
                }

                int get_music_volume() const {
                    return m_volume;
                }

                bool is_playing() const {
                    return mmActive();
                }

                void pause_music() {
                    mmPause();
                }

                void resume_music() {
                    mmResume();
                }

                void set_music_pitch(const int pitch) {
                    m_pitch = pitch;

                    mmSetModulePitch(pitch);
                }

                void set_music_tempo(const int tempo) {
                    m_tempo = tempo;

                    mmSetModuleTempo(tempo);
                }

                void set_music_volume(const int volume) {
                    m_volume = volume;

                    mmSetModuleVolume(volume);
                }

                void stop_music() {
                    mmStop();
                }


                void play_music(bool looped);
            protected:
                int get_sound_bank_ref_num() const {
                    return m_sound_bank_ref_num;
                }

                bool is_loaded() const {
                    return m_loaded;
                }

                virtual void load_music() = 0;
            private:
                bool m_loaded = false;
                int m_sound_bank_ref_num;
                int m_pitch = 1024;
                int m_tempo = 1024;
                int m_volume = 1024;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_MAX_MOD_MUSIC_HPP
