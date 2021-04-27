//
// Created by bobby on 01/04/2021.
//

#include "text_base.hpp"

#include <utils.hpp>

morpheus::core::gfx::TextBase::TextBase(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                                        morpheus::core::MainLoop *main_loop, bool use_native_text_api) {
    m_affine = affine;
    m_background_num = background_num;
    m_cbb = cbb;
    m_main_loop = main_loop;
    m_sbb = sbb;
    m_use_native_text_api = use_native_text_api;
}

bool morpheus::core::gfx::TextBase::init_expression_text_api() {
    switch(m_font.font_bpp) {
        case FontBpp::FONT_4BPP:
            m_expression_background.reset(utils::construct_appropriate_tiled_background_4bpp(false, get_background_num(),
                                                                                             nullptr,nullptr,
                                                                                             m_main_loop, get_cbb(),get_sbb()));
            break;
        case FontBpp::FONT_8BPP:
            m_expression_background.reset(utils::construct_appropriate_tiled_background_8bpp(false, get_background_num(),
                                                                                             nullptr,nullptr,
                                                                                             m_main_loop, get_cbb(),get_sbb()));
            break;
    }

    pal_bg_mem[15] = 0x7FFF;

    m_expression_background->load_from_array(m_font.font_tiles, m_font.font_tiles_len, nullptr, 0, nullptr, 0,
                                             TiledBackgroundSize::BG_64x64);

    return true;
}

void morpheus::core::gfx::TextBase::expression_print_chars(std::string string) {

    for(int i = 0; m_font.char_size.get_x() > i; i += 8) {
        //
    }
}
