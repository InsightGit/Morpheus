//
// Created by bobby on 02/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_MAX_MOD_MUSIC_HPP
#define MORPHEUS_GBA_TEST_NDS_MAX_MOD_MUSIC_HPP

#include <morpheus/core/audio/max_mod_music.hpp>

namespace morpheus {
    namespace nds {
        namespace audio {
            class NdsMaxModMusic : public morpheus::core::audio::MaxModMusic {
            public:
                /// Constructs an NdsMaxModMusic object for MaxMod module
                /// playback and optionally initializes the MaxMod library
                /// with a sound bank. Note that you must initialize MaxMod
                /// once using the second argument for successful playback.
                /// \param sound_bank_ref_num Reference number for the module
                /// this NdsMaxModMusic will play.
                /// \param sound_bank The optional void pointer to the sound bank
                /// that all future MaxMod API usages will use.
                NdsMaxModMusic(int sound_bank_ref_num, void *sound_bank = nullptr) :
                    morpheus::core::audio::MaxModMusic(sound_bank_ref_num) {
                    if(sound_bank != nullptr) {
                        mmInitDefaultMem(sound_bank);
                    }
                }

                /// Destructs the NdsMaxModMusic object.
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

            /// \class morpheus::gba::audio::NdsMaxModMusic
            /// The Nintendo DS implementation of
            /// morpheus::core::audio::MaxModMusic. For more about this class's
            /// behavior, see the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_MAX_MOD_MUSIC_HPP
