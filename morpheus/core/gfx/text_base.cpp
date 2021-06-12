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
                                                                                             nullptr, nullptr,
                                                                                             m_main_loop, get_cbb(),
                                                                                             get_sbb()));
            break;
        case FontBpp::FONT_8BPP:
            m_expression_background.reset(utils::construct_appropriate_tiled_background_8bpp(false, get_background_num(),
                                                                                             nullptr,nullptr,
                                                                                             m_main_loop, get_cbb(),
                                                                                             get_sbb()));
            break;
    }

    m_expression_background->load_from_array(m_font.font_tiles, m_font.font_tiles_len, m_font.font_palette,
                                             m_font.font_palette_len, nullptr, 0,
                                             TiledBackgroundSize::BG_64x64);

    return true;
}

void morpheus::core::gfx::TextBase::expression_print_chars(std::string string) {
    for(unsigned char c : string) {
        int tile_id = c - m_font.ascii_offset;

        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("Tile id for " + std::to_string(c) + " is " +
                                                                          std::to_string(tile_id));

        if(tile_id < 0 || c == m_font.space_ascii_code) {
            continue;
        }

        if((m_bounding_box.get_x() > 0 &&
            m_cursor_position.get_x() >= m_print_position.get_x() + m_bounding_box.get_x()) ||
           c == m_font.new_line_ascii_code) {
            m_cursor_position = Vector2(m_print_position.get_x(),
                                        m_cursor_position.get_y() + (m_font.char_size.get_y() * 8));
        }

        if(m_bounding_box.get_y() > 0 &&
           m_cursor_position.get_y() >= m_print_position.get_y() + m_bounding_box.get_y()) {
            break;
        }

        for(int y = 0; m_font.char_size.get_y() > y; ++y) {
            for(int x = 0; m_font.char_size.get_x() > x; ++x) {
                Vector2 text_position = m_cursor_position + Vector2(x, y);

                if(m_font.is_2d_mapping) {
                    tile_id += (m_font.char_size.get_y() * (16 / m_font.char_size.get_y())) + m_font.char_size.get_x();
                } else {
                    tile_id += m_font.char_size.get_y() + m_font.char_size.get_x();
                }

                m_expression_background->set_tile_id_at_position(text_position, tile_id);

                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("Printing " + std::to_string(tile_id)
                                                                                  + " at " + text_position.to_string());
            }
        }

        m_cursor_position = Vector2(m_cursor_position.get_x() + (m_font.char_size.get_x() * 8),
                                    m_cursor_position.get_y());
    }
}
