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

void morpheus::core::gfx::Window::add_background(unsigned int background) {
    m_active_backgrounds.push_back(background);

    if(is_window_enabled()) {
        // to update window settings
        toggle_window(true);
    }
}

void morpheus::core::gfx::Window::disable_blending() {
    m_blending_enabled = false;

    if(is_window_enabled()) {
        toggle_window(true);
    }
}

void morpheus::core::gfx::Window::disable_objects() {
    m_objects_enabled = false;

    if(is_window_enabled()) {
        toggle_window(true);
    }
}

void morpheus::core::gfx::Window::disable_window() {
    if(is_window_enabled()) {
        toggle_window(false);

        m_main_loop->disable_window(m_window_type);

        m_window_enabled = false;
    }
}

void morpheus::core::gfx::Window::enable_blending() {
    m_blending_enabled = true;

    if(is_window_enabled()) {
        toggle_window(true);
    }
}

void morpheus::core::gfx::Window::enable_objects() {
    m_objects_enabled = true;

    if(is_window_enabled()) {
        toggle_window(true);
    }
}

void morpheus::core::gfx::Window::enable_window() {
    if(!is_window_enabled()) {
        toggle_window(true);

        m_main_loop->enable_window(m_window_type);

        m_window_enabled = true;
    }
}

std::vector<unsigned int> morpheus::core::gfx::Window::get_backgrounds() const {
    return m_active_backgrounds;
}
