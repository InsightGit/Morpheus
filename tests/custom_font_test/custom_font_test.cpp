//
// Created by bobby on 22/04/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif


#include "Stick-Regular-en.h"
#include "Stick-Regular-jp.h"
//#include "Montserrat-Light-en.h"
//#include "Roboto-Black-en.h"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    std::shared_ptr<morpheus::core::gfx::TextBase> text_base(morpheus::utils::construct_appropriate_text(
                                                                                                     false, 0, 1, 1,
                                                                                                     main_loop.get()));

    morpheus::core::gfx::Font stick_english_font = {
            .font_palette = Stick_Regular_enPal,
            .font_palette_len = 32,
            .font_tiles = Stick_Regular_enTiles,
            .font_tiles_len = Stick_Regular_enTilesLen,
            .ascii_offset = 32,
            .char_size = morpheus::core::gfx::Vector2(2, 2),
            .font_bpp = morpheus::core::gfx::FontBpp::FONT_4BPP,
            .is_2d_mapping = true,
            .new_line_ascii_code = 10,
            .space_ascii_code = 32,
            .use_utf8 = false
    };
    morpheus::core::gfx::Font stick_japanese_font = {
            .font_palette = Stick_Regular_jpPal,
            .font_palette_len = 32,
            .font_tiles = Stick_Regular_jpTiles,
            .font_tiles_len = Stick_Regular_jpTilesLen,
            .use_utf8 = true,
            .utf8_map = Stick_Regular_jpUtfMap
    };

    text_base->set_current_font(stick_japanese_font);

    //text_base->print_at_pos("Hello World!", morpheus::core::gfx::Vector2(0, 32));

    text_base->print_at_pos(u8"こんにちは", morpheus::core::gfx::Vector2(0, 64));

    main_loop->enable_background(0);

    main_loop->game_loop();
}
