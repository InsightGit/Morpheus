//
// Created by bobby on 05/02/2021.
//

#include "window.hpp"

morpheus::core::gfx::Window::Window(const morpheus::core::gfx::WindowType window_type,
                                    const std::shared_ptr<core::MainLoop> main_loop) {
    m_main_loop = main_loop;
    m_window_type = window_type;

    m_window_rect.bottom = 0;
    m_window_rect.left = 0;
    m_window_rect.right = 0;
    m_window_rect.top = 0;
}

void morpheus::core::gfx::Window::disable_window() {
    if(m_window_enabled) {
        toggle_window(false);

        m_main_loop->disable_window(m_window_type);

        m_window_enabled = false;
    }
}

void morpheus::core::gfx::Window::enable_window() {
    if(!m_window_enabled) {
        toggle_window(true);

        m_main_loop->enable_window(m_window_type);

        m_window_enabled = true;
    }
}
