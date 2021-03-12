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
                GbaMaxModMusic(int sound_bank_ref_num, void *sound_bank = nullptr,
                               unsigned char num_of_channels = -1) :
                    morpheus::core::audio::MaxModMusic(sound_bank_ref_num) {
                    if(sound_bank != nullptr && num_of_channels > 0 && num_of_channels <= 4) {
                        mmInitDefault(sound_bank, num_of_channels);
                    }
                }

                virtual ~GbaMaxModMusic() = default;
            protected:
                void load_music() override {}
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_MAX_MOD_MUSIC_HPP
