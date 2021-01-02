//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_GBA_MAIN_LOOP_HPP
#define MORPHEUS_GBA_MAIN_LOOP_HPP

#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <tonc.h>

#include <core/main_loop.hpp>

namespace morpheus {
    namespace gba {
        static constexpr uint16_t GBA_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L };
        static constexpr int GBA_KEYPAD_BITS_SIZE = 10;
        static constexpr int GBA_MAX_SPRITES = 128;

        enum class DebugConsoleMode {
            USE_DEFAULT, // on when compiled in debug mode, off otherwise
            ON, // on no matter what
            OFF, // no console shown no matter what
        };

        class GbaMainLoop : public core::MainLoop {
        public:
            explicit GbaMainLoop(DebugConsoleMode debug_console_mode);

            virtual ~GbaMainLoop();

            [[noreturn]] core::Error game_loop() override;
        protected:
            core::Error platform_init() override;

            core::InputEvent to_input_event(uint32_t inputs, uint16_t keypad_bit,
                                            morpheus::core::InputState input_state)override;
        private:
            class DebugStream : std::ostringstream {
            public:
                void refresh_and_print();
                std::streambuf *rdbuf_string_stream() {
                    return rdbuf();
                }
            };

            std::unique_ptr<DebugStream> m_debug_stream;
            std::vector<void *> m_obj_buffer;

            void setup_debug_console();
        };
    }
}

#endif //MORPHEUS_GBA_MAIN_LOOP_HPP