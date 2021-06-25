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
#include "Stick_Regular_jp_utf_map.hpp"
//#include "Montserrat-Light-en.h"
//#include "Roboto-Black-en.h"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    std::shared_ptr<morpheus::core::gfx::TextBase> en_text_base(morpheus::utils::construct_appropriate_text(
            false, 1, 3, 3,
            main_loop.get()));
    std::shared_ptr<morpheus::core::gfx::TextBase> jp_text_base(morpheus::utils::construct_appropriate_text(
                                                                                                     false, 0, 1, 1,
                                                                                                     main_loop.get()));

    morpheus::core::gfx::Font stick_english_font(Stick_Regular_enPal, 32, Stick_Regular_enTiles,
                                                 Stick_Regular_enTilesLen, morpheus::core::gfx::Vector2(2, 2),
                                                 morpheus::core::gfx::FontBpp::FONT_1BPP, true, 32);
    morpheus::core::gfx::Font stick_japanese_font(Stick_Regular_jpPal, 32, Stick_Regular_jpTiles,
                                                  Stick_Regular_jpTilesLen,morpheus::core::gfx::Vector2(2, 2),
                                                  morpheus::core::gfx::FontBpp::FONT_1BPP, true,
                                                  Stick_Regular_jpUtfMap);

    en_text_base->set_current_font(stick_english_font);
    jp_text_base->set_current_font(stick_japanese_font);

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("printing english");
    en_text_base->print_at_pos("Hello World!", morpheus::core::gfx::Vector2(0, 32));

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("printing japanese");
    jp_text_base->print_at_pos(u8"こんにちは!", morpheus::core::gfx::Vector2(0, 64));

    en_text_base->print_at_pos("Good Morning.", morpheus::core::gfx::Vector2(0, 96));
    jp_text_base->print_at_pos(u8"おはようございます。", morpheus::core::gfx::Vector2(0, 128));

    main_loop->enable_background(0);
    main_loop->enable_background(1);

    main_loop->game_loop();
}
