//
// Created by bobby on 01/04/2021.
//

#ifndef MORPHEUS_TEXT_BASE_HPP
#define MORPHEUS_TEXT_BASE_HPP

#include <core/main_loop.hpp>
#include <core/gfx/tiled_background_base.hpp>
#include <core/gfx/vector_2.hpp>

//#include "sys8.h"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class FontBpp {
                FONT_4BPP,
                FONT_8BPP
            };

            struct Font {
                const unsigned int *font_tiles;
                unsigned int font_tiles_len;
                const unsigned short *font_palette;
                unsigned int font_palette_len;


                unsigned int ascii_offset;
                Vector2 char_size; // Char size (in tiles, NOT pixels)
                Vector2 cursor_position;
                FontBpp font_bpp;
                bool is_2d_mapping;
                unsigned int new_line_ascii_code = 10;
                unsigned int space_ascii_code = 32;
            };

            class TextBase {
            public:
                // TODO(Bobby): Add custom font support maybe?
                TextBase(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                         morpheus::core::MainLoop *main_loop, bool use_native_text_api);

                virtual ~TextBase() = default;

                unsigned int get_background_num() const {
                    return m_background_num;
                }

                Vector2 get_bounding_box() const {
                    return m_bounding_box;
                }

                unsigned int get_cbb() const {
                    return m_cbb;
                }

                Vector2 get_print_position() const {
                    return m_print_position;
                }

                unsigned int get_sbb() const {
                    return m_sbb;
                }

                bool is_affine() const {
                    return m_affine;
                }

                void set_bounding_box(Vector2 bounding_box) {
                    m_bounding_box = bounding_box;
                }

                void set_print_position(Vector2 print_pos) {
                    m_print_position = print_pos;

                    change_print_position(m_print_position);
                }

                void print_at_pos(std::string string, Vector2 print_pos) {
                    change_print_position(print_pos);

                    print(string);

                    change_print_position(m_print_position);
                }

                void print(std::string string, bool reinit = false) {
                    if(m_use_native_text_api) {
                        expression_print_chars(string);
                    } else {
                        if(reinit || !m_inited) {
                            m_main_loop->enable_background(m_background_num);

                            print_chars(string, true);
                        } else {
                            print_chars(string, false);
                        }
                    }

                    m_inited = true;
                }
            protected:
                virtual void change_print_position(Vector2 print_pos) = 0;
                virtual void print_chars(std::string string, bool init) = 0;

                static Font get_default_font() {
                    Font font;

                    font.ascii_offset = 32;
                    font.char_size = Vector2(8, 8);
                    font.cursor_position = Vector2(0, 0);
                    font.font_bpp = FontBpp::FONT_4BPP;
                    //font.font_tiles = sys8Tiles;
                    //font.font_tiles_len = sys8TilesLen;
                    font.font_palette = nullptr;

                    return font;
                }

                void expression_print_chars(std::string string);
                bool init_expression_text_api();
            private:
                bool m_affine;
                unsigned int m_background_num;
                Vector2 m_bounding_box = Vector2(0, 0);
                unsigned int m_cbb;
                Vector2 m_cursor_position;
                std::shared_ptr<core::gfx::TiledBackgroundBase> m_expression_background;
                Font m_font = get_default_font();
                bool m_inited;
                Vector2 m_print_position;
                core::MainLoop *m_main_loop;
                unsigned int m_sbb;
                bool m_use_native_text_api;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_BASE_HPP
