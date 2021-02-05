//
// Created by bobby on 04/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_MAX_MOD_SFX_HPP
#define MORPHEUS_GBA_TEST_NDS_MAX_MOD_SFX_HPP

#include <core/audio/max_mod_sfx.hpp>

namespace morpheus {
    namespace nds {
        namespace audio {
            class NdsMaxModSfx : public core::audio::MaxModSfx {
            public:
                NdsMaxModSfx(int sound_bank_ref_num, void *sound_bank) : core::audio::MaxModSfx(sound_bank_ref_num) {
                    if(sound_bank != nullptr) {
                        mmInitDefaultMem(sound_bank);
                    }
                }

                virtual ~NdsMaxModSfx() {
                    mmUnloadEffect(get_sound_bank_ref_num());
                }
            protected:
                void load_effect() override {
                    mmLoadEffect(get_sound_bank_ref_num());
                }
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_NDS_MAX_MOD_SFX_HPP
