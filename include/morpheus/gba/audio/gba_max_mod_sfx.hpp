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
                GbaMaxModSfx(int sound_bank_ref_num, void *sound_bank = nullptr,
                             int num_of_channels = -1) :
                    morpheus::core::audio::MaxModSfx(sound_bank_ref_num) {
                    if(sound_bank != nullptr && num_of_channels > 0 && num_of_channels <= 8) {
                        mmInitDefault(sound_bank, num_of_channels);
                    }
                }

                virtual ~GbaMaxModSfx() = default;
            protected:
                void load_effect() override {}
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_MAX_MOD_SFX_HPP
