//
// Created by bobby on 25/11/2020.
//

#ifndef MORPHEUS_NDS_MAIN_LOOP_HPP
#define MORPHEUS_NDS_MAIN_LOOP_HPP

#include <nds.h>

#include <core/main_loop.hpp>

namespace morpheus {
    namespace nds {

        static constexpr uint16_t NDS_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L,
                                                        /* DS exclusive keys */
                                                        KEY_X, KEY_Y, KEY_TOUCH, KEY_LID };
        static constexpr int NDS_KEYPAD_BITS_SIZE = 14;

        class NdsMainLoop : public core::MainLoop {
        public:
            [[noreturn]] core::Error game_loop() override;
        protected:
            core::Error platform_init() override;
        };
    }
}

#endif //MORPHEUS_NDS_MAIN_LOOP_HPP
