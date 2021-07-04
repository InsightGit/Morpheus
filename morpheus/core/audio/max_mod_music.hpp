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
                /// Constructs a MaxModMusic object with a MaxMod-generated
                /// reference number to a specific sound bank to use.
                /// \param sound_bank_ref_num The MaxMod sound bank reference
                /// number to use
                MaxModMusic(int sound_bank_ref_num) {
                    m_sound_bank_ref_num = sound_bank_ref_num;
                }

                /// Deconstructs a MaxModMusic object.
                virtual ~MaxModMusic() {
                    mmStop();
                }

                /// \return The pitch of the module being played (or going to
                /// be played) in this MaxModMusic class.
                int get_music_pitch() const {
                    return m_pitch;
                }

                /// \return The tempo of the module being played (or going to
                /// be played).
                int get_music_tempo() const {
                    return m_tempo;
                }

                /// \return The volume of the module being played (or going to
                /// be played).
                int get_music_volume() const {
                    return m_volume;
                }

                /// \return Whether the module loaded in this MaxModMusic class
                /// is paused.
                bool is_paused() const {
                    return m_paused;
                }

                /// \return Whether the module loaded in this MaxModMusic class
                /// is playing.
                bool is_playing() const {
                    return mmActive();
                }

                /// Pauses this MaxMod module if it is currently playing.
                void pause_music() {
                    if(is_playing()) {
                        m_paused = true;

                        mmPause();
                    }
                }

                /// Resumes this MaxMod module if it is currently paused.
                void resume_music() {
                    if(is_paused()) {
                        m_paused = true;

                        mmResume();
                    }
                }

                /// Sets the pitch of the module being played
                /// (or going to be played).
                /// \param pitch The new pitch to set on this class's module
                void set_music_pitch(const int pitch) {
                    m_pitch = pitch;

                    mmSetModulePitch(pitch);
                }

                /// Sets the tempo of the module being played
                /// (or going to be played). Can be inclusively
                /// between 512 and 2048.
                /// \param tempo The new tempo on this class's module
                /// \return Whether the tempo value passed in was in range
                /// and successfully set.
                bool set_music_tempo(const int tempo) {
                    if(tempo >= 512 && tempo <= 2048) {
                        m_tempo = tempo;

                        mmSetModuleTempo(tempo);

                        return true;
                    } else {
                        return false;
                    }
                }

                /// Sets the volume of the module being played
                /// (or going to be played). Can be inclusively
                /// between 0 and 1024.
                /// \param volume The new volume on this class's module
                /// \return Whether the volume value passed in was in range
                /// and successfully set.
                bool set_music_volume(const int volume) {
                    if(volume >= 0 && volume <= 1024) {
                        m_volume = volume;

                        mmSetModuleVolume(volume);

                        return true;
                    } else {
                        return false;
                    }
                }

                /// Stops this class's module's playback.
                void stop_music() {
                    if(is_playing()) {
                        mmStop();
                    }
                }

                /// Plays this class's module and optionally loops it.
                /// \param looped Whether to loop this class module (true)
                /// or not (false)
                void play_music(bool looped);
            protected:
                /// \return The reference number of the MaxMod sound bank being
                /// used by this class.
                int get_sound_bank_ref_num() const {
                    return m_sound_bank_ref_num;
                }

                /// \return Whether a module was loaded into this class.
                bool is_loaded() const {
                    return m_loaded;
                }

                /// Pure virtual function that loads a MaxMod module into this
                /// class in a platform-specific way.
                virtual void load_music() = 0;
            private:
                bool m_loaded = false;
                int m_sound_bank_ref_num;
                bool m_paused = false;
                int m_pitch = 1024;
                int m_tempo = 1024;
                int m_volume = 1024;
            };


            /// \class morpheus::core::audio::MaxModMusic
            /// A wrapper class for MaxMod's module playback API with adjustable
            /// pitch, tempo and volume. The required MaxMod sound bank should
            /// be able to be generated through the generate_maxmod_soundbank
            /// function in cmake/morpheus_build_utils.cmake. There should
            /// only be one MaxModMusic active and playing at a time, otherwise
            /// undefined behavior will occur. For more about this class, see
            /// MaxMod Test (tests/maxmod_test/maxmod_test.cpp) and
            /// [MaxMod documentation](https://maxmod.devkitpro.org/ref/).
        }
    }
}

#endif //MORPHEUS_GBA_TEST_MAX_MOD_MUSIC_HPP
