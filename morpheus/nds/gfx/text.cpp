//
// Created by bobby on 19/02/2021.
//

#include "text.hpp"

void morpheus::nds::gfx::Text::print_chars(std::string string, bool init) {
    if(init) {
        BgSize bg_size = BgSize_T_256x256;

        switch (m_background_size) {
            case TextBackgroundSize::BGSIZE_32_X_32:
                bg_size = BgSize_T_256x256;
                break;
            case TextBackgroundSize::BGSIZE_32_X_64:
                bg_size = BgSize_T_256x512;
                break;
            case TextBackgroundSize::BGSIZE_64_X_32:
                bg_size = BgSize_T_512x256;
                break;
            case TextBackgroundSize::BGSIZE_64_X_64:
                bg_size = BgSize_T_512x512;
                break;
        }

        if(m_use_sub_display) {
            vramSetBankA(VRAM_A_MAIN_BG);
            m_main_loop->enable_background(get_background_num());
        } else {
            vramSetBankC(VRAM_C_SUB_BG);
            m_main_loop->enable_background(get_background_num() + 4);
        }

        m_print_console.fontCurPal = 16;

        consoleInit(&m_print_console, static_cast<int>(get_background_num()), BgType_Text4bpp, bg_size,
                    static_cast<int>(get_sbb()), static_cast<int>(get_cbb()), !m_use_sub_display,
                    !m_font_loaded);

        std::string display_message = "on ";

        if(m_use_sub_display) {
            display_message += "sub display";
        } else {
            display_message += "main display";
        }

        nocashMessage(std::string("Print console loaded on background " +
                                           std::to_string(get_background_num()) + " and " +
                                           display_message).c_str());

        if(!m_font_loaded) {
            m_font_loaded = true;
        }

        nocashMessage("inited console");
    } else {
        consoleSelect(&m_print_console);

        nocashMessage("selected console");
    }

    std::cout << string;

    nocashMessage(std::string("on console: " + string).c_str());
}
