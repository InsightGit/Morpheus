//
// Created by bobby on 07/04/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "test4.h"

class InputNode : public morpheus::core::ControlReciever {
public:
    InputNode(std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite_base,
              std::shared_ptr<morpheus::core::gfx::TextBase> text_base) {
        m_sprite_base = sprite_base;
        m_text_base = text_base;
    }

    void input(const morpheus::core::InputEvent input_event) {
        if(input_event.state == morpheus::core::InputState::DOWN) {
            switch (input_event.button) {
                case morpheus::core::InputButton::START:
                    m_rotation_enabled = !m_rotation_enabled;
                    break;
                default:
                    break;
            }

            if(m_rotation_enabled) {
                rotation_input(input_event);
            } else {
                scale_input(input_event);
            }
        }
    }

    void update(const unsigned char cycle_time) {
        morpheus::core::gfx::Vector2 base_position = m_sprite_base->get_position();

        m_text_base->print_at_pos("               ",
                                  morpheus::core::gfx::Vector2(64, 0));
        m_text_base->print_at_pos("rotation: " + std::to_string(m_sprite_base->get_rotation()),
                                  morpheus::core::gfx::Vector2(64, 0));

        m_text_base->print_at_pos("               ", morpheus::core::gfx::Vector2(64, 8));
        m_text_base->print_at_pos("scale: " + m_sprite_base->get_scale().to_string(),
                                  morpheus::core::gfx::Vector2(64, 8));
    }
private:
    void rotation_input(const morpheus::core::InputEvent input_event) {
        int rotation = m_sprite_base->get_rotation();

        switch(input_event.button) {
            case morpheus::core::InputButton::DPADLEFT:
                m_sprite_base->set_rotation(rotation - 128);
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                m_sprite_base->set_rotation(rotation + 128);
                break;
            default:
                break;
        }
    }

    void scale_input(const morpheus::core::InputEvent input_event) {
        morpheus::core::gfx::Vector2 scale = m_sprite_base->get_scale();

        switch(input_event.button) {
            case morpheus::core::InputButton::DPADUP:
                m_sprite_base->set_scale(morpheus::core::gfx::Vector2(scale.get_x(), scale.get_y() + 4));
                break;
            case morpheus::core::InputButton::DPADDOWN:
                m_sprite_base->set_scale(morpheus::core::gfx::Vector2(scale.get_x(), scale.get_y() - 4));
                break;
            case morpheus::core::InputButton::DPADLEFT:
                m_sprite_base->set_scale(morpheus::core::gfx::Vector2(scale.get_x() - 4, scale.get_y()));
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                m_sprite_base->set_scale(morpheus::core::gfx::Vector2(scale.get_x() + 4, scale.get_y()));
                break;
            default:
                break;
        }
    }

    bool m_rotation_enabled = true;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
    std::shared_ptr<morpheus::core::gfx::TextBase> m_text_base;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    nocash_puts("affine init started");

    std::shared_ptr<morpheus::core::gfx::SpriteBase> base_sprite(morpheus::utils::construct_appropriate_sprite_4bpp(
                                                          true, nullptr, nullptr));
    std::shared_ptr<morpheus::core::gfx::TextBase> info_text(morpheus::utils::construct_appropriate_text(false,
                                                                                                         1, 25, 28));
    std::shared_ptr<InputNode> input_node(new InputNode(base_sprite, info_text));

    nocash_puts("affine init finish");

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite4Bpp*>(base_sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen, test4Pal, test4PalLen, 0,
                morpheus::core::gfx::SpriteSize::SIZE_32X32, 2);
    #elif _NDS
    #endif

    base_sprite->set_position(64, 64);

    main_loop->add_control_reciever(input_node);
    nocash_puts(std::string("Sprite addition result: " +
                                std::to_string(main_loop->add_sprite(base_sprite))).c_str());

    main_loop->enable_affine(morpheus::core::gfx::AffineMode::MIXED_AFFINE);
    main_loop->enable_background(1);

    main_loop->game_loop();
}
