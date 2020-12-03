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

        class GbaMainLoop : public core::MainLoop {
        public:
            [[noreturn]] core::Error game_loop() override;
        protected:
            core::Error platform_init() override;
        private:
           OBJ_ATTR m_obj_buffer[128];
        };
    }
}

#endif //MORPHEUS_GBA_MAIN_LOOP_HPP
