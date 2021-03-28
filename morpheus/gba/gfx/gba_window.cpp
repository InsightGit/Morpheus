//
// Created by bobby on 05/02/2021.
//

#include "gba_window.hpp"

void morpheus::gba::gfx::GbaWindow::toggle_window(bool on) {
    if(on) {
        std::vector<unsigned int> active_backgrounds = get_backgrounds();
        morpheus::core::gfx::WindowRect window_rect = get_window_rect();

        switch(get_window_type()) {
            case morpheus::core::gfx::WindowType::WINDOW_0:
                REG_WIN0H = window_rect.left << 8 | window_rect.right;
                REG_WIN0V = window_rect.top << 8 | window_rect.bottom;

                break;
            case core::gfx::WindowType::WINDOW_1:
                REG_WIN1H = window_rect.left << 8 | window_rect.right;
                REG_WIN1V = window_rect.top << 8 | window_rect.bottom;

                break;
            case core::gfx::WindowType::WINDOW_OBJ:
                break;
            case core::gfx::WindowType::WINDOW_OUT:
                break;
        }

        for(unsigned int i = 0; active_backgrounds.size() > i; ++i) {
            unsigned char window_register_value = 0x0;

            switch(active_backgrounds[i]) {
                case 0:
                    window_register_value = WIN_BG0;
                    break;
                case 1:
                    window_register_value = WIN_BG1;
                    break;
                case 2:
                    window_register_value = WIN_BG2;
                    break;
                case 3:
                    window_register_value = WIN_BG3;
                    break;
            }

            switch(get_window_type()) {
                case morpheus::core::gfx::WindowType::WINDOW_0:
                    REG_WININ |= window_register_value;
                    break;
                case core::gfx::WindowType::WINDOW_1:
                    REG_WININ |= window_register_value << 8;
                    break;
                case core::gfx::WindowType::WINDOW_OBJ:
                    REG_WINOUT |= window_register_value << 8;
                    break;
                case core::gfx::WindowType::WINDOW_OUT:
                    REG_WINOUT |= window_register_value;
                    break;
            }
        }
    } else {
        switch(get_window_type()) {
            case core::gfx::WindowType::WINDOW_0:
                REG_WININ &= 0xFF00;

                REG_WIN0H = 0x0;
                REG_WIN0V = 0x0;

                break;
            case core::gfx::WindowType::WINDOW_1:
                REG_WININ &= 0x00FF;

                REG_WIN1H = 0x0;
                REG_WIN1V = 0x0;

                break;
            case core::gfx::WindowType::WINDOW_OBJ:
                REG_WINOUT &= 0x00FF;
                break;
            case core::gfx::WindowType::WINDOW_OUT:
                REG_WINOUT &= 0xFF00;
                break;
        }
    }
}
