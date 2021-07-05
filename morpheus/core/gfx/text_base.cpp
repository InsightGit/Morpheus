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
    auto unpacking_needed = BitUnpacking::NONE;

    switch(m_font.get_font_bpp()) {
        case FontBpp::FONT_1BPP:
            unpacking_needed = BitUnpacking::BPP_1_TO_4;
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

    m_expression_background->load_from_array(m_font.get_font_tiles(), m_font.get_font_tiles_len(),
                                             m_font.get_font_palette(),m_font.get_font_palette_len(),
                                             nullptr, 0,
                                             TiledBackgroundSize::BG_64x64, unpacking_needed);

    return true;
}

void morpheus::core::gfx::TextBase::expression_print_chars(std::string string) {
    std::vector<int> tile_ids;

    m_current_new_line_indices.clear();

    if(m_font.is_using_utf8()) {
        tile_ids = get_tile_ids_from_utf_string(string);
    } else {
        tile_ids = get_tile_ids_from_ascii_string(string);
    }

    for(unsigned int i = 0; tile_ids.size() > i; ++i) {
        if((m_bounding_box.get_x() > 0 &&
            m_cursor_position.get_x() >= m_print_position.get_x() + m_bounding_box.get_x()) ||
                std::find(m_current_new_line_indices.begin(), m_current_new_line_indices.end(),
                          i) != m_current_new_line_indices.end()) {
            m_cursor_position = Vector2(m_print_position.get_x() - (m_font.get_char_size().get_x() * 8),
                                        m_cursor_position.get_y() + (m_font.get_char_size().get_y() * 8));
        }

        if(m_bounding_box.get_y() > 0 &&
           m_cursor_position.get_y() >= m_print_position.get_y() + m_bounding_box.get_y()) {
            break;
        }

        for(int y = 0; m_font.get_char_size().get_y() > y; ++y) {
            for(int x = 0; m_font.get_char_size().get_x() > x; ++x) {
                int subtile_id = tile_ids[i];
                Vector2 text_position = m_cursor_position + Vector2(x * 8, y * 8);

                if(static_cast<unsigned int>(subtile_id) != m_font.get_space_ascii_code()) {
                    if(m_font.is_using_2d_mapping()) {
                        if(y > 0) {
                            subtile_id += y * 16;

                            /*m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                                    "new after adding y " + std::to_string(subtile_id));*/
                        }

                        subtile_id += x;

                        /*m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                                "new after adding x " + std::to_string(subtile_id));*/
                    } else {
                        subtile_id += x + y;

                        /*m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                                "new after adding xy " + std::to_string(subtile_id));*/
                    }
                }

                m_expression_background->set_tile_id_at_position(text_position, subtile_id);

                /*m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("Printing " + std::to_string(subtile_id)
                + " at " + text_position.to_string() + "\n");*/
            }
        }

        m_cursor_position = Vector2(m_cursor_position.get_x() + (m_font.get_char_size().get_x() * 8),
                                    m_cursor_position.get_y());

        //m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("\n\n\n");
    }
}

std::vector<int> morpheus::core::gfx::TextBase::get_tile_ids_from_ascii_string(const std::string &string) {
    std::vector<int> return_value;

    for(unsigned int i = 0; string.length() > i; ++i) {
        auto c = static_cast<unsigned char>(string[i]);
        int ascii_difference = c - m_font.get_ascii_offset();
        int tile_id;

        if(c == m_font.get_new_line_ascii_code()) {
            m_current_new_line_indices.push_back(i);

            if(ascii_difference < 0) {
                return_value.push_back(0);
                continue;
            }
        }

        if(m_font.is_using_2d_mapping()) {
            tile_id = (m_font.get_char_size().get_x() * (ascii_difference % (16 / m_font.get_char_size().get_x()))) +
                    ((16 * m_font.get_char_size().get_y()) * (ascii_difference / 8));
        } else {
            tile_id = ascii_difference * (m_font.get_char_size().get_x() * m_font.get_char_size().get_x());
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
        const std::map<unsigned int, unsigned int> &utf8_map = m_font.get_utf8_map();

        auto tile_id_iterator = utf8_map.find(c);

        if(tile_id_iterator == utf8_map.end()) {
            m_main_loop->get_no_cash_debug_controller()->send_to_debug_window(
                    std::string("Couldn't find " + std::to_string(static_cast<unsigned int>(c))).c_str());
        } else {
            return_value.push_back(tile_id_iterator->second);
        }
    }

    return return_value;
}
