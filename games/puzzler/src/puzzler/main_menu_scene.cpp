//
// Created by bobby on 06/03/2021.
//

#include "main_menu_scene.hpp"

puzzler::MainMenuScene::MainMenuScene(morpheus::core::MainLoop *main_loop) : puzzler::Scene(main_loop) {
    m_cursor.reset(morpheus::utils::construct_appropriate_sprite_4bpp(false, nullptr, nullptr));
    m_main_bg.reset(morpheus::utils::construct_appropriate_tiled_background_4bpp(false, 1, nullptr, nullptr,
                                                                                 main_loop, MAIN_BG_CBB_NUM,
                                                                                 MAIN_BG_SBB_NUM));
    #ifdef _NDS
        m_sub_bg.reset(morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 1, nullptr, nullptr, main_loop,
                                                                                       MAIN_BG_CBB_NUM, MAIN_BG_SBB_NUM, true));
    #endif
}

puzzler::MainMenuScene::~MainMenuScene() {
    #ifdef _GBA
        tte_set_pos(56, 40);

        tte_write("                        ");

        tte_set_pos(88, 64);

        tte_write("                        ");

        tte_set_pos(88, 88);

        tte_write("                        ");

        tte_set_pos(88, 104);

        tte_write("                        ");
    #elif _NDS
        consoleInit(&m_title_console, 0, BgType_Text4bpp, BgSize_T_256x256, TITLE_TEXT_SBB_NUM, TITLE_TEXT_CBB_NUM,
                    true, false);

        consoleClear();

        morpheus::nds::NdsMainLoop::reset_to_debug_print_console();
    #endif

    get_main_loop()->remove_sprite(m_cursor);
}


void puzzler::MainMenuScene::setup() {
    m_main_bg->load_from_array(mainmenuscreenTiles, mainmenuscreenTilesLen, mainmenuscreenPal, mainmenuscreenPalLen,
                               mainmenuscreenMap, mainmenuscreenMapLen,
                               morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
    #ifdef _GBA
        auto *cursor_sprite = static_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_cursor.get());

        cursor_sprite->load_from_array(reinterpret_cast<const unsigned short *>(menucursorTiles), menucursorTilesLen,
                                       circlejewelPal, circlejewelPalLen, 0,
                                       morpheus::core::gfx::SpriteSize::SIZE_16X16, 16);

        tte_init_se(0, BG_CBB(TITLE_TEXT_CBB_NUM) | BG_SBB(TITLE_TEXT_SBB_NUM), 0, CLR_WHITE,
                    14, nullptr, nullptr);

        tte_set_pos(56, 40);

        tte_write("Morpheus Puzzler");

        tte_set_pos(88, 64);

        tte_write("Easy board");

        tte_set_pos(88, 88);

        tte_write("Medium board");

        tte_set_pos(88, 104);

        tte_write("Hard board");

        get_main_loop()->add_sprite(m_cursor);

        cursor_sprite->set_position(MENU_POSITIONS[0]);
    #elif _NDS
        auto *cursor_sprite = static_cast<morpheus::nds::gfx::Sprite4Bpp*>(m_cursor.get());

        // intentional copy
        unsigned short old_palette_value = BG_PALETTE[0];

        cursor_sprite->load_from_array(reinterpret_cast<const unsigned short *>(menucursorTiles), menucursorTilesLen,
                                       circlejewelPal, circlejewelPalLen, 4,
                                       morpheus::core::gfx::SpriteSize::SIZE_16X16);
        m_sub_bg->load_from_array(submainmenuscreenTiles, submainmenuscreenTilesLen, mainmenuscreenPal,
                                  mainmenuscreenPalLen, submainmenuscreenMap, submainmenuscreenMapLen,
                                  morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

        consoleInit(&m_title_console, 0, BgType_Text4bpp, BgSize_T_256x256, TITLE_TEXT_SBB_NUM, TITLE_TEXT_CBB_NUM,
                    true, true);

        std::cout << "\x1b[5;6H Morpheus Puzzler";
        std::cout << "\x1b[8;9H Easy board";
        std::cout << "\x1b[11;9H Medium board";
        std::cout << "\x1b[14;9H Hard board";

        BG_PALETTE[0] = old_palette_value;

        cursor_sprite->set_position(MENU_POSITIONS[0]);

        get_main_loop()->add_sprite(m_cursor);

        morpheus::nds::NdsMainLoop::reset_to_debug_print_console();
    #endif

    get_main_loop()->enable_background(0);
}

void puzzler::MainMenuScene::input(morpheus::core::InputEvent input_event) {
    if(input_event.state == morpheus::core::InputState::DOWN) {
        switch(input_event.button) {
            case morpheus::core::InputButton::DPADUP:
                if(m_cursor_position > 0) {
                    m_cursor_position = m_cursor_position - 1;
                }
                break;
            case morpheus::core::InputButton::DPADDOWN:
                m_cursor_position = std::min(2u, m_cursor_position + 1);
                break;
            case morpheus::core::InputButton::A:
            case morpheus::core::InputButton::START:
                mark_for_deletion();
                break;
            default:
                break;
        }

        #ifdef _GBA
            static_cast<morpheus::gba::gfx::Sprite*>(m_cursor.get())->set_position(MENU_POSITIONS[m_cursor_position]);
        #elif _NDS
            static_cast<morpheus::nds::gfx::Sprite*>(m_cursor.get())->set_position(MENU_POSITIONS[m_cursor_position]);
        #endif
    }
}

void puzzler::MainMenuScene::update(unsigned char cycle_time) {
    if(m_cursor_animation_timer.current_action_cycle_waiting &&
       m_cursor_animation_timer.current_action_cycle == cycle_time) {
        if(m_cursor_lit) {
            #ifdef _GBA
                static_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_cursor.get())->set_palette_id(4);
            #elif _NDS
                static_cast<morpheus::nds::gfx::Sprite*>(m_cursor.get())->set_palette_id(4);
            #endif
        } else {
            #ifdef _GBA
                static_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_cursor.get())->set_palette_id(5);
            #elif _NDS
                static_cast<morpheus::nds::gfx::Sprite*>(m_cursor.get())->set_palette_id(5);
            #endif
        }

        m_cursor_animation_timer.current_action_cycle = cycle_time + 30;
        m_cursor_lit = !m_cursor_lit;

        if(m_cursor_animation_timer.current_action_cycle >= 60) {
            m_cursor_animation_timer.current_action_cycle -= 60;
        }
    } else if(!m_cursor_animation_timer.current_action_cycle_waiting) {
        m_cursor_animation_timer.current_action_cycle = 30;
        m_cursor_animation_timer.current_action_cycle_waiting = true;
    }
}
