//
// Created by bobby on 02/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_MAX_MOD_MUSIC_HPP
#define MORPHEUS_GBA_TEST_NDS_MAX_MOD_MUSIC_HPP

#include <core/audio/max_mod_music.hpp>

namespace morpheus {
    namespace nds {
        namespace audio {
            class NdsMaxModMusic : public morpheus::core::audio::MaxModMusic {
            public:
                NdsMaxModMusic(int sound_bank_ref_num, void *sound_bank = nullptr) :
                    morpheus::core::audio::MaxModMusic(sound_bank_ref_num) {
                    if(sound_bank != nullptr) {
                        mmInitDefaultMem(sound_bank);
                    }
                }

                virtual ~NdsMaxModMusic() {
                    if(is_loaded()) {
                        mmUnload(get_sound_bank_ref_num());
                    }
                }
            protected:
                void load_music() override {
                    mmLoad(get_sound_bank_ref_num());
                }
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_MAX_MOD_MUSIC_HPP
