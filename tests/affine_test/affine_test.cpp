//
// Created by bobby on 07/04/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "region_map_affine_128.h"
#include "region_map_affine.h"
#include "region_map_affine2.h"
//#include "nums.h"
#include "test4.h"

class InputNode : public morpheus::core::ControlReciever {
public:
    InputNode(morpheus::core::NoCashDebugController *no_cash_debug_controller,
              std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite_base,
              std::shared_ptr<morpheus::core::gfx::TextBase> text_base,
              std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> tiled_background_base) {
        m_no_cash_debug_controller = no_cash_debug_controller;
        m_sprite_base = sprite_base;
        m_text_base = text_base;
        m_tiled_background_base = tiled_background_base;
    }

    void input(const morpheus::core::InputEvent input_event) {
        if(input_event.state == morpheus::core::InputState::DOWN) {
            switch (input_event.button) {
                case morpheus::core::InputButton::SELECT:
                    if(m_input_mode > FIRST_INPUT_MODE) {
                        m_input_mode = static_cast<InputMode>(static_cast<unsigned int>(m_input_mode) - 1);
                    }
                    break;
                case morpheus::core::InputButton::START:
                    m_input_mode = static_cast<InputMode>(std::min(static_cast<unsigned int>(LAST_INPUT_MODE),
                                                                   static_cast<unsigned int>(m_input_mode) + 1));
                    break;
                default:
                    break;
            }
        }

        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            switch(m_input_mode) {
                case InputMode::SPRITE_ROTATION:
                    //m_no_cash_debug_controller->send_to_debug_window("doing sprite rotation");

                    rotation_input(input_event, true);
                    break;
                case InputMode::SPRITE_SCALE:
                    //m_no_cash_debug_controller->send_to_debug_window("doing sprite scale");

                    scale_input(input_event, true);
                    break;
                case InputMode::BACKGROUND_SCROLL:
                    scroll_input(input_event);

                    break;
                case InputMode::BACKGROUND_ROTATION:
                    //m_no_cash_debug_controller->send_to_debug_window("doing background rotation");

                    rotation_input(input_event, false);
                    break;
                case InputMode::BACKGROUND_SCALE:
                    //m_no_cash_debug_controller->send_to_debug_window("doing background scale");

                    scale_input(input_event, false);
                    break;
            }
        }
    }

    void update(const unsigned char cycle_time) {
        morpheus::core::gfx::Vector2 base_position = m_sprite_base->get_position();

        /*#ifdef _GBA
            m_text_base->print_at_pos("                     ",
                                      morpheus::core::gfx::Vector2(64, 0));
            m_text_base->print_at_pos("rotation: " + std::to_string(m_sprite_base->get_rotation()),
                                      morpheus::core::gfx::Vector2(64, 0));

            m_text_base->print_at_pos("                     ", morpheus::core::gfx::Vector2(64, 8));
            m_text_base->print_at_pos("scale: " + m_sprite_base->get_scale().to_string(),
                                      morpheus::core::gfx::Vector2(64, 8));
        #endif*/
    }
private:
    void scroll_input(const morpheus::core::InputEvent input_event) {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            morpheus::core::gfx::Vector2 scroll_pos = m_tiled_background_base->get_scroll();

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() - 10);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() - 10, scroll_pos.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() + 10, scroll_pos.get_y());
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() + 10);
                    break;
                default:
                    break;
            }

            m_tiled_background_base->set_scroll(scroll_pos);
        }
    }

    void rotation_input(const morpheus::core::InputEvent input_event, bool sprite) {
        int rotation = m_sprite_base->get_rotation();

        switch(input_event.button) {
            case morpheus::core::InputButton::DPADLEFT:
                rotation += 128;
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                rotation -= 128;
                break;
            default:
                break;
        }

        if(sprite) {
            m_sprite_base->set_rotation(rotation);
        } else {
            m_tiled_background_base->set_rotation(rotation);
        }
    }

    void scale_input(const morpheus::core::InputEvent input_event, bool sprite) {
        morpheus::core::gfx::Vector2 scale = m_sprite_base->get_scale();

        switch(input_event.button) {
            case morpheus::core::InputButton::DPADUP:
                scale = morpheus::core::gfx::Vector2(scale.get_x(), scale.get_y() + 4);
                break;
            case morpheus::core::InputButton::DPADDOWN:
                scale = morpheus::core::gfx::Vector2(scale.get_x(), scale.get_y() - 4);
                break;
            case morpheus::core::InputButton::DPADLEFT:
                scale = morpheus::core::gfx::Vector2(scale.get_x() - 4, scale.get_y());
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                scale = morpheus::core::gfx::Vector2(scale.get_x() + 4, scale.get_y());
                break;
            default:
                break;
        }

        if(sprite) {
            m_sprite_base->set_scale(scale);
        } else {
            m_tiled_background_base->set_scale(scale);
        }
    }

    const static unsigned int INPUT_MODE_GAP = 1;

    enum class InputMode {
        SPRITE_ROTATION = 0,
        SPRITE_SCALE = static_cast<int>(SPRITE_ROTATION) + INPUT_MODE_GAP,
        BACKGROUND_SCROLL = static_cast<int>(SPRITE_SCALE) + INPUT_MODE_GAP,
        BACKGROUND_ROTATION = static_cast<int>(BACKGROUND_SCROLL) + INPUT_MODE_GAP,
        BACKGROUND_SCALE = static_cast<int>(BACKGROUND_ROTATION) + INPUT_MODE_GAP
    };

    const static InputMode FIRST_INPUT_MODE = InputMode::SPRITE_ROTATION;
    const static InputMode LAST_INPUT_MODE = InputMode::BACKGROUND_SCALE;

    InputMode m_input_mode = InputMode::BACKGROUND_SCROLL;
    morpheus::core::NoCashDebugController *m_no_cash_debug_controller;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
    std::shared_ptr<morpheus::core::gfx::TextBase> m_text_base;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_tiled_background_base;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> base_background(morpheus::utils::
                            construct_appropriate_tiled_background_8bpp(true, 2, nullptr, nullptr, main_loop.get(),
                                                                        1, 1));
    std::shared_ptr<morpheus::core::gfx::SpriteBase> base_sprite(morpheus::utils::construct_appropriate_sprite_4bpp(
                                                          true, nullptr, nullptr));
    std::shared_ptr<morpheus::core::gfx::TextBase> info_text(morpheus::utils::construct_appropriate_text(false,
                                                                                                     2, 15, 15,
                                                                                                     main_loop.get(),
                                                                                                     true));
    std::shared_ptr<InputNode> input_node(new InputNode(main_loop->get_no_cash_debug_controller(),
                                                        base_sprite, info_text, base_background));


    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite4Bpp*>(base_sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen,
                reinterpret_cast<const unsigned short *>(test4Pal), 32,
                15, morpheus::core::gfx::SpriteSize::SIZE_32X32, 10);
    #elif _NDS
        static_cast<morpheus::nds::gfx::Sprite4Bpp*>(base_sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short*>(&(test4Tiles[0])), test4TilesLen,
                reinterpret_cast<const unsigned short*>(&(test4Pal[0])), test4PalLen, 0,
                morpheus::core::gfx::SpriteSize::SIZE_32X32);
    #endif

    base_background->load_from_array(region_map_affine_128Tiles, region_map_affine_128TilesLen,
                                     region_map_affine_128Pal, region_map_affine_128PalLen,
                                     region_map_affine_128Map, region_map_affine_128MapLen,
                                     morpheus::core::gfx::TiledBackgroundSize::BG_AFFINE_128x128);

    /*base_background->load_from_array(region_map_affineTiles, region_map_affineTilesLen,
                                 region_map_affinePal, region_map_affinePalLen,
                                 region_map_affineMap, region_map_affineMapLen,
                                 morpheus::core::gfx::TiledBackgroundSize::BG_AFFINE_32x32);*/

    /*base_background->load_from_array(region_map_affine2Tiles, region_map_affine2TilesLen,
                                     region_map_affine2Pal, region_map_affine2PalLen,
                                     region_map_affine2Map, region_map_affine2MapLen,
                                     morpheus::core::gfx::TiledBackgroundSize::BG_AFFINE_32x32);*/

    base_sprite->set_position(64, 64);
    base_sprite->set_affine_index(1);

    base_background->set_affine_index(2);

    main_loop->enable_affine(morpheus::core::gfx::AffineMode::FULL_AFFINE);
    //main_loop->enable_background(0);

    main_loop->add_control_reciever(input_node);

    main_loop->add_sprite(base_sprite);

    //info_text->print_at_pos("hello", morpheus::core::gfx::Vector2(32, 32));

    main_loop->game_loop();
}
