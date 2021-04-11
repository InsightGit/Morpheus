//
// Created by bobby on 22/03/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "region_map.h"
#include "region_map_window.h"
#include "test8.h"

class InputNode : public morpheus::core::ControlReciever {
public:
    InputNode(const std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> &mosaic_background,
              const std::shared_ptr<morpheus::core::gfx::SpriteBase> &mosaic_sprite,
              const std::shared_ptr<morpheus::core::gfx::Window> &window,
              morpheus::core::gfx::BlendingController *blending_controller,
              morpheus::core::NoCashDebugController *no_cash_debug_controller) {
        m_blending_controller = blending_controller;
        m_mosaic_background = mosaic_background;
        m_mosaic_sprite = mosaic_sprite;
        m_no_cash_debug_controller = no_cash_debug_controller;
        m_window = window;
    }

    void input(morpheus::core::InputEvent input_event) override {
        if(input_event.state == morpheus::core::InputState::HELD || \
               input_event.state == morpheus::core::InputState::DOWN) {
            update_control_mode(input_event);

            switch (m_control_mode) {
                case ControlMode::BACKGROUND_MOSAIC:
                    //nocash_puts("on background mosaic");
                    //m_no_cash_debug_controller->send_to_debug_window("On background mosaic mode");
                    mosaic_input(input_event, true);
                    break;
                case ControlMode::SPRITE_MOSAIC:
                    //nocash_puts("on sprite mosaic");
                    m_no_cash_debug_controller->send_to_debug_window("On sprite mosaic mode");
                    mosaic_input(input_event, false);
                    break;
                case ControlMode::BLENDING:
                    //nocash_puts("on blending");
                    m_no_cash_debug_controller->send_to_debug_window("On blending mosaic mode");
                    blending_input(input_event);
                    break;
                case ControlMode::WINDOW:
                    m_no_cash_debug_controller->send_to_debug_window("On window mode");
                    window_input(input_event);
                    break;
            }
        }
    }
    void update(unsigned char cycle_time) override {}
private:
    const static int CONTROL_MODE_GAP = 1;

    enum class ControlMode {
        BACKGROUND_MOSAIC = 0,
        SPRITE_MOSAIC = static_cast<int>(BACKGROUND_MOSAIC) + CONTROL_MODE_GAP,
        BLENDING = static_cast<int>(SPRITE_MOSAIC) + CONTROL_MODE_GAP,
        WINDOW = static_cast<int>(BLENDING) + CONTROL_MODE_GAP
    };

    const static ControlMode FINAL_CONTROL_MODE = ControlMode::WINDOW;
    const static ControlMode FIRST_CONTROL_MODE = ControlMode::BACKGROUND_MOSAIC;

    void blending_input(morpheus::core::InputEvent input_event) {
        switch(input_event.button) {
            case morpheus::core::InputButton::DPADLEFT:
                m_blending_controller->set_blend_weight(true,
                                                        m_blending_controller->get_blend_weight(true) + 1);
                m_blending_controller->set_blend_weight(false,
                                                        std::max(m_blending_controller->get_blend_weight(false) - 1, 0));
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                m_blending_controller->set_blend_weight(true,
                                                        std::max(m_blending_controller->get_blend_weight(true) - 1, 0));
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

    void window_input(morpheus::core::InputEvent input_event) {
        morpheus::core::gfx::WindowRect window_rect = m_window->get_window_rect();

        switch(input_event.button) {
            case morpheus::core::InputButton::DPADUP:
                window_rect.top -= 5;
                window_rect.bottom -= 5;
                break;
            case morpheus::core::InputButton::DPADDOWN:
                window_rect.top += 5;
                window_rect.bottom += 5;
                break;
            case morpheus::core::InputButton::DPADLEFT:
                window_rect.left -= 5;
                window_rect.right -= 5;
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                window_rect.left += 5;
                window_rect.right += 5;
                break;
            default:
                break;
        }

        m_window->set_window_rect(window_rect);
    }

    ControlMode m_control_mode = ControlMode::BACKGROUND_MOSAIC;
    morpheus::core::gfx::BlendingController *m_blending_controller;
    morpheus::core::NoCashDebugController *m_no_cash_debug_controller;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_mosaic_background;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_mosaic_sprite;
    std::shared_ptr<morpheus::core::gfx::Window> m_window;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite(
            morpheus::utils::construct_appropriate_sprite_8bpp(false, main_loop->get_blending_controller(),
                                                                  main_loop->get_mosaic_controller(), false, false));
    std::shared_ptr<morpheus::core::gfx::Window> window;
    std::shared_ptr<morpheus::core::gfx::Window> window_out;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> window_background;
    std::shared_ptr<InputNode> input_node;

    morpheus::core::gfx::WindowRect window_rect;

    window_rect.top = 36;
    window_rect.bottom = 96;

    window_rect.left = 144;
    window_rect.right = 232;

    #ifdef _GBA
        background.reset(new morpheus::gba::gfx::TiledBackground(
                1, static_cast<morpheus::gba::gfx::GbaBlendingController*>(main_loop->get_blending_controller()),
                static_cast<morpheus::gba::gfx::GbaMosaicController*>(main_loop->get_mosaic_controller()),
                static_cast<morpheus::gba::GbaMainLoop*>(main_loop.get()), true, 2, 2));
        window_background.reset(new morpheus::gba::gfx::TiledBackground(
                0, static_cast<morpheus::gba::gfx::GbaBlendingController*>(main_loop->get_blending_controller()),
                static_cast<morpheus::gba::gfx::GbaMosaicController*>(main_loop->get_mosaic_controller()),
                static_cast<morpheus::gba::GbaMainLoop*>(main_loop.get()), true, 2, 10));

        window.reset(new morpheus::gba::gfx::GbaWindow(morpheus::core::gfx::WindowType::WINDOW_0,
                                                       main_loop));
        window_out.reset(new morpheus::gba::gfx::GbaWindow(morpheus::core::gfx::WindowType::WINDOW_OUT,
                                                           main_loop));

        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                reinterpret_cast<const unsigned short *>(test8Pal), test8PalLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    #elif _NDS
        background.reset(new morpheus::nds::gfx::TiledBackground8Bpp(
                         false, false, 1,
                         static_cast<morpheus::nds::gfx::NdsBlendingController*>(main_loop->get_blending_controller()),
                         static_cast<morpheus::nds::gfx::NdsMosaicController*>(main_loop->get_mosaic_controller()),
                         static_cast<morpheus::nds::NdsMainLoop*>(main_loop.get()), 2, 2));
        window_background.reset(new morpheus::nds::gfx::TiledBackground8Bpp(
                false, false, 0, static_cast<morpheus::nds::gfx::NdsBlendingController*>(main_loop->get_blending_controller()),
                static_cast<morpheus::nds::gfx::NdsMosaicController*>(main_loop->get_mosaic_controller()),
                static_cast<morpheus::nds::NdsMainLoop*>(main_loop.get()), 2, 10));

        window.reset(new morpheus::nds::gfx::NdsWindow(false,
                                                       morpheus::core::gfx::WindowType::WINDOW_0,
                                                       main_loop));
        window_out.reset(new morpheus::nds::gfx::NdsWindow(false, morpheus::core::gfx::WindowType::WINDOW_OUT,
                                                           main_loop));

        static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                                                reinterpret_cast<const unsigned short *>(&test8Tiles[0]), test8TilesLen,
                                                reinterpret_cast<const unsigned short *>(&test8Pal[0]), test8PalLen,
                                                morpheus::core::gfx::SpriteSize::SIZE_32X32);

        nocashMessage("Loaded from array");
    #endif

    input_node.reset(new InputNode(background, sprite, window, main_loop->get_blending_controller(),
                                      main_loop->get_no_cash_debug_controller()));

    background->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                region_mapMap, region_mapMapLen, morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
    window_background->load_from_array(region_map_windowTiles, region_map_windowTilesLen, region_map_windowMap,
                                       region_map_windowMapLen,morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    background->toggle_mosaic();
    sprite->toggle_mosaic();

    background->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 1));
    sprite->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 1));

    sprite->set_position(50, 50);

    background->enable_blending(true);
    sprite->enable_blending(false);

    window->set_window_rect(window_rect);
    window->add_background(1);

    window_out->add_background(0);
    window_out->enable_objects();

    window->enable_window();
    window_out->enable_window();

    main_loop->add_sprite(sprite);

    main_loop->add_control_reciever(input_node);

    main_loop->get_blending_controller()->enable_background_blending(true, 0);

    main_loop->get_blending_controller()->set_blending_mode(morpheus::core::gfx::BlendingMode::USE_WEIGHTS);
    main_loop->get_blending_controller()->set_blend_weight(false, 8);
    main_loop->get_blending_controller()->set_blend_weight(true, 8);

    main_loop->game_loop();
}
