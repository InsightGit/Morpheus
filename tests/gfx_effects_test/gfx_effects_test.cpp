//
// Created by bobby on 22/03/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "region_map.h"
//#include "region_map_window.h"
#include "test8.h"

class InputNode : public morpheus::core::Node {
public:
    InputNode(const std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> &mosaic_background,
              const std::shared_ptr<morpheus::core::gfx::SpriteBase> &mosaic_sprite,
              morpheus::core::gfx::BlendingController *blending_controller) {
        m_blending_controller = blending_controller;
        m_mosaic_background = mosaic_background;
        m_mosaic_sprite = mosaic_sprite;
    }
protected:
    void input(morpheus::core::InputEvent input_event) override {
        if(input_event.state == morpheus::core::InputState::HELD || \
               input_event.state == morpheus::core::InputState::DOWN) {
            update_control_mode(input_event);

            switch (m_control_mode) {
                case ControlMode::BACKGROUND_MOSAIC:
                    nocash_puts("On background mosaic mode");
                    mosaic_input(input_event, true);
                    break;
                case ControlMode::SPRITE_MOSAIC:
                    nocash_puts("On sprite mosaic mode");
                    mosaic_input(input_event, false);
                    break;
                case ControlMode::BLENDING:
                    nocash_puts("On blending mosaic mode");
                    blending_input(input_event);
                    break;
            }
        }
    }
    void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) override {}
    void on_visible_state_changed(bool new_visible_state) override {}
    void update(unsigned char cycle_time) override {}
private:
    const static int CONTROL_MODE_GAP = 1;

    enum class ControlMode {
        BACKGROUND_MOSAIC = 0,
        SPRITE_MOSAIC = static_cast<int>(BACKGROUND_MOSAIC) + CONTROL_MODE_GAP,
        BLENDING = static_cast<int>(SPRITE_MOSAIC) + CONTROL_MODE_GAP
    };

    const static ControlMode FINAL_CONTROL_MODE = ControlMode::BLENDING;
    const static ControlMode FIRST_CONTROL_MODE = ControlMode::BACKGROUND_MOSAIC;

    void blending_input(morpheus::core::InputEvent input_event) {
        switch(input_event.button) {
            case morpheus::core::InputButton::DPADLEFT:
                m_blending_controller->set_blend_weight(true,
                                                        m_blending_controller->get_blend_weight(true) + 1);
                m_blending_controller->set_blend_weight(false,
                                                        m_blending_controller->get_blend_weight(false) - 1);
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                m_blending_controller->set_blend_weight(true,
                                                        m_blending_controller->get_blend_weight(true) - 1);
                m_blending_controller->set_blend_weight(false,
                                                        m_blending_controller->get_blend_weight(false) + 1);
                break;
            default:
                break;
        }
    }

    void mosaic_input(morpheus::core::InputEvent input_event, bool background) {
        morpheus::core::gfx::Vector2 mosaic_levels;

        if(background) {
            mosaic_levels = m_mosaic_background->get_mosaic_levels();
        } else {
            mosaic_levels = m_mosaic_sprite->get_mosaic_levels();
        }

        switch(input_event.button) {
            case morpheus::core::InputButton::DPADUP:
                mosaic_levels = morpheus::core::gfx::Vector2(mosaic_levels.get_x(), mosaic_levels.get_y() + 1);
                break;
            case morpheus::core::InputButton::DPADDOWN:
                mosaic_levels = morpheus::core::gfx::Vector2(mosaic_levels.get_x(), mosaic_levels.get_y() - 1);
                break;
            case morpheus::core::InputButton::DPADLEFT:
                mosaic_levels = morpheus::core::gfx::Vector2(mosaic_levels.get_x() - 1, mosaic_levels.get_y());
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                mosaic_levels = morpheus::core::gfx::Vector2(mosaic_levels.get_x() + 1, mosaic_levels.get_y());
                break;
            default:
                break;
        }

        if(background) {
            m_mosaic_background->set_mosaic_levels(mosaic_levels);
        } else {
            m_mosaic_sprite->set_mosaic_levels(mosaic_levels);
        }
    }

    void update_control_mode(morpheus::core::InputEvent input_event) {
        switch(input_event.button) {
            case morpheus::core::InputButton::START: {
                int control_value_int = static_cast<int>(m_control_mode);

                if(control_value_int >= static_cast<int>(FINAL_CONTROL_MODE)) {
                    m_control_mode = FIRST_CONTROL_MODE;
                } else {
                    m_control_mode = static_cast<ControlMode>(control_value_int + CONTROL_MODE_GAP);
                }

                break;
            }
            case morpheus::core::InputButton::SELECT: {
                int control_value_int = static_cast<int>(m_control_mode);

                if(control_value_int <= static_cast<int>(FIRST_CONTROL_MODE)) {
                    m_control_mode = FINAL_CONTROL_MODE;
                } else {
                    m_control_mode = static_cast<ControlMode>(control_value_int - CONTROL_MODE_GAP);
                }

                break;
            }
            default:
                break;
        }
    }

    ControlMode m_control_mode = ControlMode::SPRITE_MOSAIC;
    morpheus::core::gfx::BlendingController *m_blending_controller;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_mosaic_background;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_mosaic_sprite;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite(
            morpheus::utils::construct_appropriate_sprite_8bpp(main_loop->get_blending_controller(),
                                                               false, true));
    std::shared_ptr<morpheus::core::gfx::Window> window;
    std::shared_ptr<InputNode> input_node;

    /*morpheus::core::gfx::WindowRect window_rect;

    window_rect.top = 36;
    window_rect.bottom = 76;

    window_rect.left = 20;
    window_rect.right = 60;*/

    #ifdef _GBA
        background.reset(new morpheus::gba::gfx::TiledBackground(
                0, static_cast<morpheus::gba::gfx::GbaBlendingController*>(main_loop->get_blending_controller()),
                static_cast<morpheus::gba::GbaMainLoop*>(main_loop.get()), true, 2, 2));

        window.reset(new morpheus::gba::gfx::GbaWindow(morpheus::core::gfx::WindowType::WINDOW_0,
                                                       main_loop));

        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                reinterpret_cast<const unsigned short *>(test8Pal), test8PalLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    #elif _NDS
        background.reset(new morpheus::nds::gfx::TiledBackground8Bpp(
                         false, 0,
                         static_cast<morpheus::nds::gfx::NdsBlendingController*>(main_loop->get_blending_controller()),
                         static_cast<morpheus::nds::NdsMainLoop*>(main_loop.get()), 5, 5));

        window.reset(new morpheus::nds::gfx::NdsWindow(false,
                                                       morpheus::core::gfx::WindowType::WINDOW_0,
                                                       main_loop));

        static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                                                reinterpret_cast<const unsigned short *>(&test8Tiles[0]), test8TilesLen,
                                                reinterpret_cast<const unsigned short *>(&test8Pal[0]), test8PalLen,
                                                morpheus::core::gfx::SpriteSize::SIZE_32X32);

        nocashMessage("Loaded from array");
    #endif

    input_node.reset(new InputNode(background, sprite, main_loop->get_blending_controller()));

    background->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                region_mapMap, region_mapMapLen, morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    background->toggle_mosaic();
    sprite->toggle_mosaic();

    background->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 1));
    sprite->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 1));

    sprite->set_position(50, 50);

    background->enable_blending(true);
    sprite->enable_blending(false);

    //window->set_window_rect()

    input_node->add_child(sprite.get());

    main_loop->set_root(input_node);

    main_loop->get_blending_controller()->set_blending_mode(morpheus::core::gfx::BlendingMode::USE_WEIGHTS);
    main_loop->get_blending_controller()->set_blend_weight(false, 8);
    main_loop->get_blending_controller()->set_blend_weight(true, 8);

    main_loop->game_loop();
}
