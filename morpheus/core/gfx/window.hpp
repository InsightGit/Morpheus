//
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
                unsigned char bottom;
                unsigned char left;
                unsigned char right;
                unsigned char top;
            };

            enum class WindowType {
                WINDOW_0,
                WINDOW_1,
                WINDOW_OBJ,
                WINDOW_OUT
            };


            class Window {
            public:
                explicit Window(const WindowType window_type, const std::shared_ptr<core::MainLoop> main_loop);

                virtual ~Window() = default;

                WindowRect get_window_rect() const {
                    return m_window_rect;
                };

                WindowType get_window_type() const {
                    return m_window_type;
                };

                void set_window_rect(const WindowRect window_rect) {
                    m_window_rect = window_rect;
                }

                bool is_window_enabled() const {
                    return m_window_enabled;
                }

                void add_background(unsigned int background);
                void disable_window();
                void enable_window();
                std::vector<unsigned int> get_backgrounds()const;
            protected:
                virtual void toggle_window(bool on) = 0;
            private:
                std::vector<unsigned int> m_active_backgrounds;
                std::shared_ptr<core::MainLoop> m_main_loop;
                bool m_window_enabled;
                WindowRect m_window_rect;
                WindowType m_window_type;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_CORE_WINDOW_HPP
