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
                /// Constructs an NdsWindow with a WindowType, a smart
                /// pointer to the MainLoop, and a boolean indicating
                /// whether this NdsWindow will be on the bottom sub
                /// display (true) or on the top main display (false).
                /// For more information about this constructor,
                /// see the constructor of the parent class
                /// morpheus::core::gfx::Window.
                /// \param use_sub_display Whether this Window will be
                /// displayed on the bottom sub display or not.
                /// \param window_type The WindowType to use
                /// \param main_loop The smart pointer to the MainLoop to use
                NdsWindow(bool use_sub_display, const core::gfx::WindowType window_type,
                       const std::shared_ptr<core::MainLoop> main_loop) : core::gfx::Window(window_type, main_loop) {
                    m_use_sub_display = use_sub_display;
                }

                /// \return Whether this NdsWindow is on the bottom sub display
                /// of the DS.
                bool is_on_sub_display() const {
                    return m_use_sub_display;
                }
            protected:
                void toggle_window(bool on)override;
            private:
                bool m_use_sub_display;
            };

            /// \class morpheus::nds::gfx::NdsWindow
            /// The Nintendo DS implementation of
            /// morpheus::core::gfx::Window, with support for
            /// windowing on the bottom sub screen. For more about this class's
            /// behavior, see the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_WINDOW_HPP
