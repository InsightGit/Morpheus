//
// Created by bobby on 07/04/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "region_map.h"
#include "test4.h"

class InputNode : public morpheus::core::ControlReciever {
public:
    InputNode(std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite_base,
              std::shared_ptr<morpheus::core::gfx::TextBase> text_base,
              std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> tiled_background_base) {
        m_sprite_base = sprite_base;
        m_text_base = text_base;
        m_tiled_background_base = tiled_background_base;
    }

    void input(const morpheus::core::InputEvent input_event) {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            switch (input_event.button) {
                case morpheus::core::InputButton::SELECT:
                    if(m_input_mode != FIRST_INPUT_MODE) {
                        m_input_mode = static_cast<InputMode>(static_cast<unsigned int>(m_input_mode) - 1);
                    }
                    break;
                case morpheus::core::InputButton::START:
                    if(m_input_mode != LAST_INPUT_MODE) {
                        m_input_mode = static_cast<InputMode>(static_cast<unsigned int>(m_input_mode) + 1);
                    }
                    break;
                default:
                    break;
            }

            switch(m_input_mode) {
                case InputMode::SPRITE_ROTATION:
                    rotation_input(input_event, true);
                    break;
                case InputMode::SPRITE_SCALE:
                    scale_input(input_event, true);
                    break;
                case InputMode::BACKGROUND_ROTATION:
                    rotation_input(input_event, false);
                    break;
                case InputMode::BACKGROUND_SCALE:
                    scale_input(input_event, false);
                    break;
            }
        }
    }

    void update(const unsigned char cycle_time) {
        /*morpheus::core::gfx::Vector2 base_position = m_sprite_base->get_position();

        m_text_base->print_at_pos("               ",
                                  morpheus::core::gfx::Vector2(64, 0));
        m_text_base->print_at_pos("rotation: " + std::to_string(m_sprite_base->get_rotation()),
                                  morpheus::core::gfx::Vector2(64, 0));

        m_text_base->print_at_pos("               ", morpheus::core::gfx::Vector2(64, 8));
        m_text_base->print_at_pos("scale: " + m_sprite_base->get_scale().to_string(),
                                  morpheus::core::gfx::Vector2(64, 8));*/
    }
private:
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
        BACKGROUND_ROTATION = static_cast<int>(SPRITE_SCALE) + INPUT_MODE_GAP,
        BACKGROUND_SCALE = static_cast<int>(BACKGROUND_ROTATION) + INPUT_MODE_GAP
    };

    const static InputMode FIRST_INPUT_MODE = InputMode::SPRITE_ROTATION;
    const static InputMode LAST_INPUT_MODE = InputMode::BACKGROUND_SCALE;

    InputMode m_input_mode = FIRST_INPUT_MODE;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
    std::shared_ptr<morpheus::core::gfx::TextBase> m_text_base;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_tiled_background_base;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    morpheus::core::NoCashDebugController *no_cash_controller = main_loop->get_no_cash_debug_controller();

    no_cash_controller->send_to_debug_window("affine init started");

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> base_background(morpheus::utils::
                            construct_appropriate_tiled_background_8bpp(true, 2, nullptr, nullptr, main_loop.get(),
                                                                        2, 2));
    std::shared_ptr<morpheus::core::gfx::SpriteBase> base_sprite(morpheus::utils::construct_appropriate_sprite_4bpp(
                                                          true, nullptr, nullptr));
    std::shared_ptr<morpheus::core::gfx::TextBase> info_text(morpheus::utils::construct_appropriate_text(false,
                                                                                                         1, 25, 28));
    std::shared_ptr<InputNode> input_node(new InputNode(base_sprite, info_text, base_background));

    no_cash_controller->send_to_debug_window("affine init finish");

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite4Bpp*>(base_sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen, test4Pal, test4PalLen, 0,
                morpheus::core::gfx::SpriteSize::SIZE_32X32, 2);
    #elif _NDS
        static_cast<morpheus::nds::gfx::Sprite4Bpp*>(base_sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short*>(&(test4Tiles[0])), test4TilesLen,
                reinterpret_cast<const unsigned short*>(&(test4Pal[0])), test4PalLen, 0,
                morpheus::core::gfx::SpriteSize::SIZE_32X32);
    #endif

    base_background->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                     region_mapMap, region_mapMapLen,
                                     morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    base_sprite->set_position(64, 64);
    base_sprite->set_affine_index(1);

    base_background->set_affine_index(2);

    main_loop->enable_affine(morpheus::core::gfx::AffineMode::MIXED_AFFINE);
    main_loop->enable_background(1);

    main_loop->add_control_reciever(input_node);

    no_cash_controller->send_to_debug_window(std::string("Sprite addition result: " +
                                             std::to_string(main_loop->add_sprite(base_sprite))));

    main_loop->game_loop();
}
