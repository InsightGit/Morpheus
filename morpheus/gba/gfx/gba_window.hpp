//
// Created by bobby on 05/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_WINDOW_HPP
#define MORPHEUS_GBA_TEST_GBA_WINDOW_HPP

#include <tonc.h>

#include <core/core.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class GbaWindow : public core::gfx::Window {
            public:
                GbaWindow(const core::gfx::WindowType window_type,
                          const std::shared_ptr<core::MainLoop> main_loop) :
                          core::gfx::Window(window_type, main_loop) {}
            protected:
                void toggle_window(bool on)override;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_WINDOW_HPP
