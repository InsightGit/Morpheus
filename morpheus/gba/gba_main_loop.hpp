//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_GBA_MAIN_LOOP_HPP
#define MORPHEUS_GBA_MAIN_LOOP_HPP

#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <maxmod.h>
#include <tonc.h>

#include <core/main_loop.hpp>
#include <core/gfx/window.hpp>

#include <gba/gba_controllers.hpp>
#include <gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        static constexpr uint16_t GBA_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L };
        static constexpr int GBA_KEYPAD_BITS_SIZE = 10;
        static constexpr int GBA_MAX_SPRITES = 128;

        enum class DebugConsoleMode {
            USE_DEFAULT, // on when compiled in debug mode, off otherwise
            ON, // on no matter what
            NOCASH_ONLY, // only sends to nocash
            OFF, // no console shown no matter what
        };

        class GbaMainLoop : public core::MainLoop {
        public:
            const static int OBJ_ATTR_SIZE = sizeof(OBJ_ATTR) / 8;

            explicit GbaMainLoop(DebugConsoleMode debug_console_mode);

            virtual ~GbaMainLoop();

            void clear_obj_vram()override;

            void disable_window(core::gfx::WindowType window_type)override;

            void enable_background(unsigned int background_num)override;
            void enable_window(core::gfx::WindowType window_type)override;

            void send_to_debug_window(std::string string) override {
                nocash_puts(string.c_str());
            }

            [[noreturn]] core::Error game_loop() override;
        protected:
            core::Error platform_init() override;

            core::InputEvent to_input_event(uint32_t inputs, uint16_t keypad_bit,
                                            morpheus::core::InputState input_state)override;
        private:
            class DebugStream : std::ostringstream {
            public:
                DebugStream(GbaMainLoop *main_loop) {
                    m_main_loop = main_loop;
                }

                void refresh_and_print();
                std::streambuf *rdbuf_string_stream() {
                    return rdbuf();
                }
            private:
                GbaMainLoop *m_main_loop;
            };

            std::unique_ptr<DebugStream> m_debug_stream;
            std::vector<void *> m_obj_buffer;

            unsigned int m_backgrounds_to_enable = 0x0;
            unsigned int m_last_input_size = 0;
            bool m_platform_inited = false;
            bool m_using_tte = false;
            unsigned int m_windows_to_enable = 0x0;

            void setup_debug_console(bool use_tte);
        };
    }
}

#endif //MORPHEUS_GBA_MAIN_LOOP_HPP
