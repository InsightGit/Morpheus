//
// Created by bobby on 25/11/2020.
//

#ifndef MORPHEUS_NDS_MAIN_LOOP_HPP
#define MORPHEUS_NDS_MAIN_LOOP_HPP

#include <stdio.h>

#include <nds.h>

#include <core/main_loop.hpp>

namespace morpheus {
    namespace nds {

        static constexpr uint16_t NDS_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L,
                                                        /* DS exclusive keys */
                                                        KEY_X, KEY_Y, KEY_TOUCH, KEY_LID };
        static constexpr int NDS_KEYPAD_BITS_SIZE = 14;

        enum class DebugConsoleMode {
            USE_DEFAULT_MAIN, // on main/top screen when compiled in debug mode, off otherwise
            USE_DEFAULT_SUB, // on sub/bottom screen when compiled in debug mode, off otherwise
            ON_MAIN, // on main/top screen, no matter what
            ON_SUB, // on sub/bottom screen, no matter what
            OFF, // no console shown, no matter what
        };

        class NdsMainLoop : public core::MainLoop {
        public:
            explicit NdsMainLoop(DebugConsoleMode debug_console_mode = DebugConsoleMode::USE_DEFAULT_MAIN);

            [[noreturn]] core::Error game_loop() override;
        protected:
            core::Error platform_init() override;

            core::InputEvent to_input_event(uint32_t inputs, uint16_t keypad_bit,
                                            morpheus::core::InputState input_state)override;
        private:
            void setup_debug_console(bool use_main_display);
        };
    }
}

#endif //MORPHEUS_NDS_MAIN_LOOP_HPP
