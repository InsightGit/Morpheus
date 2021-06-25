//
// Created by bobby on 21/06/2021.
//

#ifndef MORPHEUS_GBA_TEST_FONT_HPP
#define MORPHEUS_GBA_TEST_FONT_HPP

#include <map>

#include <core/gfx/vector_2.hpp>

#include "sys8.h"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class FontBpp {
                FONT_1BPP,
                FONT_4BPP,
                FONT_8BPP
            };

            class Font {
            public:
                // default font
                Font() {
                    set_common_characteristics(sys8Pal, sys8PalLen, sys8Tiles, sys8TilesLen, Vector2(1, 1),
                                               FontBpp::FONT_1BPP, false);

                    m_ascii_offset = 32;
                    m_new_line_ascii_code = 10;
                    m_space_ascii_code = 32;
                    m_use_utf8 = false;
                }

                // ascii-based font constructor
                Font(const unsigned short *font_palette, const unsigned int font_palette_len,
                     const unsigned int *font_tiles, const unsigned int font_tiles_len, const Vector2 &char_size,
                     FontBpp font_bpp, bool is_2d_mapping, unsigned int ascii_offset,
                     unsigned int new_line_ascii_code = 10, unsigned int space_ascii_code = 32) {
                    set_common_characteristics(font_palette, font_palette_len, font_tiles, font_tiles_len,
                                               char_size, font_bpp, is_2d_mapping);

                    m_ascii_offset = ascii_offset;
                    m_new_line_ascii_code = new_line_ascii_code;
                    m_space_ascii_code = space_ascii_code;
                    m_use_utf8 = false;
                }

                // utf8-based font constructor
                Font(const unsigned short *font_palette, const unsigned int font_palette_len,
                     const unsigned int *font_tiles, const unsigned int font_tiles_len, const Vector2 &char_size,
                     FontBpp font_bpp, bool is_2d_mapping, const std::map<unsigned int, unsigned int> &utf8_map) {
                    set_common_characteristics(font_palette, font_palette_len, font_tiles, font_tiles_len,
                                               char_size, font_bpp, is_2d_mapping);

                    m_use_utf8 = true;
                    m_utf8_map = utf8_map;
                }

                unsigned int get_ascii_offset() const {
                    return m_ascii_offset;
                }

                const Vector2 &get_char_size() const {
                    return m_char_size;
                }

                FontBpp get_font_bpp() const {
                    return m_font_bpp;
                }

                const unsigned short *get_font_palette() const {
                    return m_font_palette;
                }

                unsigned int get_font_palette_len() const {
                    return m_font_palette_len;
                }

                const unsigned int *get_font_tiles() const {
                    return m_font_tiles;
                }

                unsigned int get_font_tiles_len() const {
                    return m_font_tiles_len;
                }

                unsigned int get_new_line_ascii_code() const {
                    return m_new_line_ascii_code;
                }

                unsigned int get_space_ascii_code() const {
                    return m_space_ascii_code;
                }

                const std::map<unsigned int, unsigned int> &get_utf8_map() const {
                    return m_utf8_map;
                }

                bool is_using_2d_mapping() const {
                    return m_is_2d_mapping;
                }

                bool is_using_utf8() const {
                    return m_use_utf8;
                }
            private:
                void set_common_characteristics(const unsigned short *font_palette, const unsigned int font_palette_len,
                                                const unsigned int *font_tiles, const unsigned int font_tiles_len,
                                                const Vector2 &char_size, FontBpp font_bpp, bool is_2d_mapping) {
                    m_char_size = char_size;
                    m_font_bpp = font_bpp;
                    m_font_palette = font_palette;
                    m_font_palette_len = font_palette_len;
                    m_font_tiles = font_tiles;
                    m_font_tiles_len = font_tiles_len;
                    m_is_2d_mapping = is_2d_mapping;
                }

                const unsigned short *m_font_palette;
                unsigned int m_font_palette_len;
                const unsigned int *m_font_tiles;
                unsigned int m_font_tiles_len;

                unsigned int m_ascii_offset;
                Vector2 m_char_size; // Char size (in tiles, NOT pixels)
                FontBpp m_font_bpp;
                bool m_is_2d_mapping;
                unsigned int m_new_line_ascii_code;// = 10;
                unsigned int m_space_ascii_code; //= 32;
                bool m_use_utf8;
                std::map<unsigned int, unsigned int> m_utf8_map;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_FONT_HPP
