//
// Created by bobby on 04/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_MAX_MOD_SFX_HPP
#define MORPHEUS_GBA_TEST_NDS_MAX_MOD_SFX_HPP

#include <morpheus/core/audio/max_mod_sfx.hpp>

namespace morpheus {
    namespace nds {
        namespace audio {
            class NdsMaxModSfx : public core::audio::MaxModSfx {
            public:
                /// Constructs an NdsMaxModSfx object for MaxMod sound
                /// effect (sfx) playback and optionally initializes the
                /// MaxMod library with a sound bank.
                /// Note that you must initialize MaxMod
                /// once using the second argument for successful playback.
                /// \param sound_bank_ref_num Reference number for the sfx
                /// this NdsMaxModSfx will play.
                /// \param sound_bank The optional void pointer to the sound bank
                /// that all future MaxMod API usages will use.
                NdsMaxModSfx(int sound_bank_ref_num, void *sound_bank) : core::audio::MaxModSfx(sound_bank_ref_num) {
                    if(sound_bank != nullptr) {
                        mmInitDefaultMem(sound_bank);
                    }
                }

                /// Destructs the NdsMaxModSfx object.
                virtual ~NdsMaxModSfx() {
                    mmUnloadEffect(get_sound_bank_ref_num());
                }
            protected:
                void load_effect() override {
                    mmLoadEffect(get_sound_bank_ref_num());
                }
            };

            /// \class morpheus::gba::audio::NdsMaxModSfx
            /// The Nintendo DS implementation of
            /// morpheus::core::audio::MaxModSfx. For more about this class's
            /// behavior, see the aforementioned parent class.
        }
    }
}


#endif //MORPHEUS_GBA_TEST_NDS_MAX_MOD_SFX_HPP
