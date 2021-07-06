//
// Created by bobby on 19/03/2021.
//

#include "morpheus/nds/gfx/nds_window.hpp"

void morpheus::nds::gfx::NdsWindow::toggle_window(bool on) {
    std::vector<unsigned int> active_backgrounds = get_backgrounds();
    WINDOW current_window;
    OamState *oam_engine;

    switch(get_window_type()) {
        case core::gfx::WindowType::WINDOW_0:
            current_window = WINDOW_0;
            break;
        case core::gfx::WindowType::WINDOW_1:
            current_window = WINDOW_1;
            break;
        case core::gfx::WindowType::WINDOW_OBJ:
            current_window = WINDOW_OBJ;
            break;
        case core::gfx::WindowType::WINDOW_OUT:
            current_window = WINDOW_OUT;
            break;
        default:
            return;
    }

    if(m_use_sub_display) {
        oam_engine = &oamSub;
    } else {
        oam_engine = &oamMain;
    }

    if(is_objects_enabled()) {
        oamWindowEnable(oam_engine, current_window);
    } else {
        oamWindowDisable(oam_engine, current_window);
    }

    for(unsigned int background : active_backgrounds) {
        if(m_use_sub_display) {
            background += 3;
        }

        if(on) {
            bgWindowEnable(static_cast<int>(background), current_window);
        } else {
            bgWindowDisable(static_cast<int>(background), current_window);
        }
    }

    if(on) {
        morpheus::core::gfx::WindowRect window_rect = get_window_rect();

        if(m_use_sub_display) {
            windowSetBoundsSub(current_window, window_rect.left, window_rect.top, window_rect.right,
                               window_rect.bottom);
        } else {
            windowSetBounds(current_window, window_rect.left, window_rect.top, window_rect.right,
                            window_rect.bottom);
        }

        if(m_use_sub_display) {
            windowEnableSub(current_window);
        } else {
            windowEnable(current_window);
        }
    } else {
        if(m_use_sub_display) {
            windowSetBoundsSub(current_window, 0, 0, 0, 0);
        } else {
            windowSetBounds(current_window, 0, 0, 0, 0);
        }

        if(m_use_sub_display) {
            windowDisableSub(current_window);
        } else {
            windowDisable(current_window);
        }
    }
}
