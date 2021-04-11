//
// Created by bobby on 19/02/2021.
//

#include "text.hpp"

void morpheus::nds::gfx::Text::print_chars(std::string string, bool init) {
    if(init) {
        BgSize bg_size = BgSize_T_256x256;

        switch (m_background_size) {
            case BackgroundSize::BGSIZE_32_X_32:
                bg_size = BgSize_T_256x256;
                break;
            case BackgroundSize::BGSIZE_32_X_64:
                bg_size = BgSize_T_256x512;
                break;
            case BackgroundSize::BGSIZE_64_X_32:
                bg_size = BgSize_T_512x256;
                break;
            case BackgroundSize::BGSIZE_64_X_64:
                bg_size = BgSize_T_512x512;
                break;
        }

        consoleInit(&m_print_console, static_cast<int>(get_background_num()), BgType_Text8bpp, bg_size,
                    static_cast<int>(get_sbb()), static_cast<int>(get_cbb()), m_use_sub_display, m_font_loaded);
    }

    if(!m_font_loaded) {
        m_font_loaded = true;
    }
}
