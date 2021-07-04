// Created by bobby on 05/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_CORE_WINDOW_HPP
#define MORPHEUS_GBA_TEST_CORE_WINDOW_HPP

#include <memory>

#include <core/main_loop.hpp>

namespace morpheus {
    namespace core {
        class MainLoop;

        namespace gfx {
            struct WindowRect {
                unsigned char bottom; ///< The window's bottom rectangular bound
                unsigned char left; ///< The window's left rectangular bound
                unsigned char right; ///< The window's right rectangular bound
                unsigned char top; ///< The window's top rectangular bound
            };


            /// \struct morpheus::core::gfx::WindowRect
            /// A struct containing the bounds of rectangular windows used by
            /// the Window class.

            enum class WindowType {
                WINDOW_0, ///< Window zero
                WINDOW_1, ///< Window one
                WINDOW_OBJ, ///< Object window
                WINDOW_OUT ///< Output window (must be set for windowing to
                           ///< work)
            };

            /// \enum morpheus::core::gfx::WindowType
            /// An enum class containing the possible types of windows
            /// the Window class supports.

            class Window {
            public:
                /// Constructs a Window class with a unique and unused type of
                /// window to control specified as well as the smart pointer to
                /// the MainLoop. If multiple Windows are constructed with the
                /// same WindowType, undefined behavior will occur.
                /// \param window_type The WindowType of this constructed Window
                /// \param main_loop The smart pointer to the main loop
                explicit Window(const WindowType window_type, const std::shared_ptr<core::MainLoop> main_loop);

                /// Deconstructs a Window.
                virtual ~Window() = default;

                /// \return This Window's rectangular bounds.
                WindowRect get_window_rect() const {
                    return m_window_rect;
                };

                /// \return The (hopefully unique) type of this Window.
                WindowType get_window_type() const {
                    return m_window_type;
                };

                /// Sets this Window's rectangular bounds with a WindowRect.
                /// \param window_rect The new rectangular bounds of this Window
                void set_window_rect(const WindowRect window_rect) {
                    m_window_rect = window_rect;

                    if(is_window_enabled()) {
                        toggle_window(true);
                    }
                }

                /// \return Whether the blending graphical effect is enabled
                /// inside this Window.
                bool is_blending_enabled() const {
                    return m_blending_enabled;
                }

                /// \return Whether objects are displayed inside this Window.
                bool is_objects_enabled() const {
                    return m_objects_enabled;
                }

                /// \return Whether this Window is enabled to be displayed at
                /// all.
                bool is_window_enabled() const {
                    return m_window_enabled;
                }

                /// Adds a specific TiledBackgroundBase to this Window,
                /// displaying it only inside this Window.
                /// \param background The background number of the
                /// TiledBackgroundBase to add this Window.
                void add_background(unsigned int background);

                /// Disables the blending graphical effect inside this Window.
                void disable_blending();

                /// Disables objects from being displayed only within this
                /// Window.
                void disable_objects();

                /// Disables this Window from being displayed at all.
                void disable_window();

                /// Enables the blending graphical effect inside this Window.
                void enable_blending();

                /// Enables objects from being displayed only within this
                /// Window.
                void enable_objects();

                /// Enables and displays this Window.
                void enable_window();

                /// \return The background numbers of the TiledBackgroundBases
                /// added to this Window.
                std::vector<unsigned int> get_backgrounds()const;
            protected:
                /// Pure virtual function that toggles this Window on or
                /// off and sets the appropriate attributes of this Window in a
                /// platform specific way. Called upon any change to the
                /// Window's enabled state and any attribute change while the
                /// Window is already enabled.
                /// \param on Whether to toggle the window on (true) or
                /// off (false).
                virtual void toggle_window(bool on) = 0;
            private:
                std::vector<unsigned int> m_active_backgrounds;
                std::shared_ptr<core::MainLoop> m_main_loop;
                bool m_blending_enabled = false;
                bool m_objects_enabled = false;
                bool m_window_enabled;
                WindowRect m_window_rect;
                WindowType m_window_type;
            };


            /// \class morpheus::core::gfx::Window
            /// A class that takes advantage of the GBA's and the DS's
            /// rectangular windowing graphical effect. This produces
            /// rectangular windows that exclusively contain certain
            /// backgrounds or objects. Note: In order for the desired effect
            /// to be achieved, both a Window with the desired WindowType and
            /// a Window with the WindowType::WINDOW_OUT must be both
            /// constructed with their WindowRects set and activated using the
            /// Window::enable_window() function. For more on this effect, look
            /// at the Graphical Effect Test
            /// (tests/gfx_effects_test/gfx_effects_test.cpp) and/or see
            /// [this webpage](https://www.coranac.com/tonc/text/gfx.htm#sec-win).
        }
    }
}

#endif //MORPHEUS_GBA_TEST_CORE_WINDOW_HPP
