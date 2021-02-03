//
// Created by bobby on 02/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_MAX_MOD_MUSIC_HPP
#define MORPHEUS_GBA_TEST_GBA_MAX_MOD_MUSIC_HPP

#include <core/audio/max_mod_music.hpp>

namespace morpheus {
    namespace gba {
        namespace audio {
            class GbaMaxModMusic : public morpheus::core::audio::MaxModMusic {
            public:
                GbaMaxModMusic(void *sound_bank, int num_of_channels, int sound_bank_ref_num) :
                    morpheus::core::audio::MaxModMusic(sound_bank_ref_num) {
                    if(sound_bank != nullptr && num_of_channels > 0) {
                        mmInitDefault(sound_bank, num_of_channels);
                    }
                }
            protected:
                void load_music() override {}
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_MAX_MOD_MUSIC_HPP
