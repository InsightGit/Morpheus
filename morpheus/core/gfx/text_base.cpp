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
    std::vector<int> tile_ids;

    if(m_font.use_utf8) {
        tile_ids = get_tile_ids_from_utf_string(string);
    } else {
        tile_ids = get_tile_ids_from_ascii_string(string);
    }

    for(int tile_id : tile_ids) {
        if((m_bounding_box.get_x() > 0 &&
            m_cursor_position.get_x() >= m_print_position.get_x() + m_bounding_box.get_x()) /*||
            c == m_font.new_line_ascii_code*/) {
            m_cursor_position = Vector2(m_print_position.get_x(),
                                        m_cursor_position.get_y() + (m_font.char_size.get_y() * 8));
        }

        if(m_bounding_box.get_y() > 0 &&
           m_cursor_position.get_y() >= m_print_position.get_y() + m_bounding_box.get_y()) {
            break;
        }

        for(int y = 0; m_font.char_size.get_y() > y; ++y) {
            for(int x = 0; m_font.char_size.get_x() > x; ++x) {
                int subtile_id = tile_id;
                Vector2 text_position = m_cursor_position + Vector2(x * 8, y * 8);

                if(static_cast<unsigned int>(subtile_id) != m_font.space_ascii_code) {
                    if(m_font.is_2d_mapping) {
                        if(y > 0) {
                            subtile_id += y * 16;

                            m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                                    "new after adding y " + std::to_string(subtile_id));
                        }

                        subtile_id += x;

                        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                                "new after adding x " + std::to_string(subtile_id));
                    } else {
                        subtile_id += x + y;

                        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                                "new after adding xy " + std::to_string(subtile_id));
                    }
                }

                m_expression_background->set_tile_id_at_position(text_position, subtile_id);

                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("Printing " + std::to_string(subtile_id)
                + " at " + text_position.to_string() + "\n");
            }
        }

        m_cursor_position = Vector2(m_cursor_position.get_x() + (m_font.char_size.get_x() * 8),
                                    m_cursor_position.get_y());

        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("\n\n\n");
    }
}

std::vector<int> morpheus::core::gfx::TextBase::get_tile_ids_from_ascii_string(const std::string &string) {
    std::vector<int> return_value;

    for(unsigned char c : string) {
        int ascii_difference = c - m_font.ascii_offset;
        int tile_id;

        if(m_font.is_2d_mapping) {
            tile_id = (m_font.char_size.get_x() * (ascii_difference % (16 / m_font.char_size.get_x()))) +
                    ((16 * m_font.char_size.get_y()) * (ascii_difference / 8));
        } else {
            tile_id = ascii_difference * 4;
        }

        /*m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("Tile id for " + std::to_string(c) +
        " is " + std::to_string(tile_id));*/

        if(tile_id < 0) {
            continue;
        }

        return_value.push_back(tile_id);
    }

    return return_value;
}

std::vector<int> morpheus::core::gfx::TextBase::get_tile_ids_from_utf_string(const std::string &string) {
    std::vector<int> return_value;
    std::u32string u32_string = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(string);

    for(char32_t c : u32_string) {
        unsigned int tile_id = m_font.utf8_map[c];

        if(tile_id == 0) {
            nocash_puts(std::string("Couldn't find " + std::to_string(static_cast<unsigned int>(c))).c_str());

            while(true) {}
        } else {
            return_value.push_back(tile_id - 1);
        }
        //return_value.push_back(m_font.utf8_map[])
    }

    return return_value;
}
