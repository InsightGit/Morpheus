//
// Created by bobby on 02/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_MAX_MOD_SFX_HPP
#define MORPHEUS_GBA_TEST_GBA_MAX_MOD_SFX_HPP

#include "../../core/audio/max_mod_sfx.hpp"

namespace morpheus {
    namespace gba {
        namespace audio {
            class GbaMaxModSfx : public core::audio::MaxModSfx {
            public:
                /// Constructs a GbaMaxModSfx object for MaxMod sound effect
                /// (sfx) playback and optionally initializes the MaxMod library
                /// with a sound bank and number of audio channels in the
                /// range [0-4]. Note that you must initialize MaxMod once
                /// using the second and third arguments for successful
                /// playback.
                /// \param sound_bank_ref_num Reference number for the sfx
                /// this GbaMaxModSfx will play.
                /// \param sound_bank The optional void pointer to the sound bank
                /// that all future MaxMod API usages will use.
                /// \param num_of_channels The optional number of audio channels
                /// that all future MaxMod sfx/modules will play on.
                GbaMaxModSfx(int sound_bank_ref_num, void *sound_bank = nullptr,
                             int num_of_channels = -1) :
                    morpheus::core::audio::MaxModSfx(sound_bank_ref_num) {
                    if(sound_bank != nullptr && num_of_channels > 0 && num_of_channels <= 8) {
                        mmInitDefault(sound_bank, num_of_channels);
                    }
                }

                /// Destructs the GbaMaxModSfx object.
                virtual ~GbaMaxModSfx() = default;
            protected:
                void load_effect() override {}
            };

            /// \class morpheus::gba::audio::GbaMaxModSfx
            /// The Game Boy Advance implementation of
            /// morpheus::core::audio::MaxModSfx. For more about this class's
            /// behavior, see the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_MAX_MOD_SFX_HPP
