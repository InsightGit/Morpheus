//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_GBA_MAIN_LOOP_HPP
#define MORPHEUS_GBA_MAIN_LOOP_HPP

#include <list>

#include <tonc.h>

#include <core/main_loop.hpp>

namespace morpheus {
    namespace gba {
        static constexpr uint16_t GBA_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L };
        static constexpr int GBA_KEYPAD_BITS_SIZE = 10;
        static constexpr int GBA_MAX_SPRITES = 128;

        class GbaMainLoop : public core::MainLoop {
        public:
            virtual ~GbaMainLoop();

            [[noreturn]] core::Error game_loop() override;
        protected:
            core::Error platform_init() override;

            core::InputEvent to_input_event(uint32_t inputs, uint16_t keypad_bit,
                                            morpheus::core::InputState input_state)override;
        private:
            std::vector<void *> m_obj_buffer;
        };
    }
}

#endif //MORPHEUS_GBA_MAIN_LOOP_HPP
