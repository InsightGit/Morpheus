//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_GBA_MAIN_LOOP_HPP
#define MORPHEUS_GBA_MAIN_LOOP_HPP

#include <gba.h>

#include <core/main_loop.hpp>

namespace morpheus {
    namespace gba {
        class GbaMainLoop : public core::MainLoop {
        public:
            core::Error game_loop() override;
        protected:
            core::Error platform_init() override;
        };
    }
}

#endif //MORPHEUS_GBA_MAIN_LOOP_HPP
