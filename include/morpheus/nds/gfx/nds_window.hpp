//
// Created by bobby on 19/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_WINDOW_HPP
#define MORPHEUS_GBA_TEST_NDS_WINDOW_HPP

#include <nds.h>

#include <morpheus/core/core.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class NdsWindow : public core::gfx::Window {
            public:
                NdsWindow(bool use_sub_display, const core::gfx::WindowType window_type,
                       const std::shared_ptr<core::MainLoop> main_loop) : core::gfx::Window(window_type, main_loop) {
                    m_use_sub_display = use_sub_display;
                }

                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }
            protected:
                void toggle_window(bool on)override;
            private:
                bool m_use_sub_display;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_WINDOW_HPP
