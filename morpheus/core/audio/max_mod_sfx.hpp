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
                /// Constructs a MaxModSfx object with a MaxMod-generated
                /// reference number to a specific sound bank to use.
                /// \param sound_bank_ref_num The MaxMod sound bank reference
                /// number to use
                MaxModSfx(unsigned int sound_bank_ref_num) {
                    m_sound_bank_ref_num = sound_bank_ref_num;
                }

                /// Destructs a MaxModSfx object
                virtual ~MaxModSfx() {
                    if(m_started) {
                        stop_effect();
                    }
                }

                /// \return The panning of this class's MaxMod sound effect.
                unsigned char get_panning() const {
                    return m_sfx_panning;
                }

                /// Sets the panning of this class's MaxMod sound effect.
                /// \param panning The new panning to set on the sound effect
                void set_panning(const unsigned char panning) {
                    m_sfx_panning = panning;

                    if(m_sfx_handle >= 0) {
                        mmEffectPanning(m_sfx_handle, panning);
                    }
                }

                /// \return The fixed-point pitch or rate that this class's
                /// MaxMod sound effect is playing at. (1024 is the original
                /// octave, 512 is half of it, 2048 is double of it, and so-on).
                unsigned short get_rate() const {
                    return m_sfx_rate;
                }

                /// Sets the fixed-point pitch or rate that this class's
                /// MaxMod sound effect is playing at. (1024 is the original
                /// octave, 512 is half of it, 2048 is double of it, and so-on).
                /// \param rate The pitch or rate to set this MaxMod sound
                /// effect to.
                void set_rate(const unsigned short rate) {
                    m_sfx_rate = rate;

                    if(m_sfx_handle >= 0) {
                        mmEffectRate(m_sfx_handle, m_sfx_rate);
                    }
                }

                /// \return The volume of this class's MaxMod sound effect
                unsigned char get_volume() const {
                    return m_sfx_volume;
                }

                /// Sets the volume of this class's MaxMod sound effect,
                /// inclusively between 0 and 255.
                /// \param volume The volume to be set on this class's MaxMod
                /// sound effect.
                void set_volume(const unsigned char volume) {
                    m_sfx_volume = volume;

                    if(m_sfx_handle >= 0) {
                        mmEffectVolume(m_sfx_handle, m_sfx_volume);
                    }
                }

                /// Stops this class's MaxMod sound effect from playing.
                void stop_effect() {
                    if(m_started) {
                        mmEffectCancel(m_sfx_handle);

                        m_started = false;
                    }
                }

                /// Starts playing this class's sound effect. Optionally can
                /// also mark this class's sound effect as low priority,
                /// allowing the effect to be interrupted by music from modules
                /// or other sound effects if needed. If it is marked as low
                /// priority, no further actions on this effect including
                /// checking the state of it will be possible.
                /// \param low_priority Whether to put this MaxMod sound
                /// effect on low priority mode or not.
                void start_effect(bool low_priority);
            protected:
                /// \return The reference number of the MaxMod sound bank being
                /// used by this class.
                unsigned int get_sound_bank_ref_num() {
                    return m_sound_bank_ref_num;
                }

                /// Pure virtual function that loads a MaxMod sound effect into
                /// this class in a platform-specific way.
                virtual void load_effect() = 0;
            private:
                bool m_loaded = false;
                bool m_started = false;

                unsigned short m_sfx_handle;
                unsigned char m_sfx_panning = 128;
                unsigned short m_sfx_rate = 1024;
                unsigned char m_sfx_volume = 255;
                unsigned int m_sound_bank_ref_num;
            };
        }


        /// \class morpheus::core::audio::MaxModSfx
        /// A wrapper class for MaxMod's sound effect playback API with
        /// adjustable playback rate/pitch, panning and volume. The required
        /// MaxMod sound bank should be able to be generated through the
        /// generate_maxmod_soundbank function in
        /// cmake/morpheus_build_utils.cmake. For more about this class, see
        /// MaxMod Test (tests/maxmod_test/maxmod_test.cpp) and
        /// [MaxMod documentation](https://maxmod.devkitpro.org/ref/).
    }
}

#endif //MORPHEUS_GBA_TEST_MAX_MOD_SFX_HPP
