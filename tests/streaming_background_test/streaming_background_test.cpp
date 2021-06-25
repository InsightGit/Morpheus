//
// Created by bobby on 15/06/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "kakariko_tiled.h"
#include "kakariko_tiled_256.h"
#include "kakariko_tiled_512.h"

class StreamingBackgroundTestControls : public morpheus::core::ControlReciever {
public:
    StreamingBackgroundTestControls(std::shared_ptr<morpheus::core::gfx::StreamingBackgroundBase> background) {
        m_background = background;
    }

    void input(morpheus::core::InputEvent input_event) override {
        scroll_background(input_event);
    }

    void update(const unsigned char cycle_time) override {}
protected:
    void scroll_background(morpheus::core::InputEvent input_event) {
        if(input_event.state == morpheus::core::InputState::DOWN ||
        input_event.state == morpheus::core::InputState::HELD) {
            morpheus::core::gfx::Vector2 scroll_pos = m_background->get_global_scroll();

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() - 8);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() - 8, scroll_pos.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() + 8, scroll_pos.get_y());
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() + 8);
                    break;
                    default:
                        break;
            }

            m_background->set_global_scroll(scroll_pos);
        }
    }
private:
    std::shared_ptr<morpheus::core::gfx::StreamingBackgroundBase> m_background;
};

int main() {
    auto main_loop = std::shared_ptr<morpheus::core::MainLoop>(morpheus::utils::construct_appropriate_main_loop());

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("Constructing TiledBackgroundBase");

    auto tiled_background_base = std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase>(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 0, nullptr, nullptr, main_loop.get(),
                                                                         3, 2, false));

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("Constructing StreamingBackgroundBase");

    auto streaming_background_base = std::shared_ptr<morpheus::core::gfx::StreamingBackgroundBase>(
            morpheus::utils::construct_appropriate_streaming_background_base(tiled_background_base.get()));


    auto control_reciever = std::shared_ptr<StreamingBackgroundTestControls>(
            new StreamingBackgroundTestControls(streaming_background_base));

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("Loading StreamingBackgroundBase");

    streaming_background_base->load_from_arrays(kakariko_tiledTiles, kakariko_tiledTilesLen,
                                                kakariko_tiledPal, kakariko_tiledPalLen,
                                                kakariko_tiledMapsRows, kakariko_tiledMapsLen,
                                                morpheus::core::gfx::StreamingBackgroundSize::BG_128x128);

    /*streaming_background_base->load_from_arrays(kakariko_tiled_256Tiles, kakariko_tiled_256TilesLen,
                                                kakariko_tiled_256Pal, kakariko_tiled_256PalLen,
                                                kakariko_tiled_256MapsRows, kakariko_tiled_256MapsLen,
                                                morpheus::core::gfx::StreamingBackgroundSize::BG_256x256);*/

    /*streaming_background_base->load_from_arrays(kakariko_tiled_512Tiles, kakariko_tiled_512TilesLen,
                                                kakariko_tiled_512Pal, kakariko_tiled_512PalLen,
                                                kakariko_tiled_512MapsRows, kakariko_tiled_512MapsLen,
                                                morpheus::core::gfx::StreamingBackgroundSize::BG_512x512);*/

    main_loop->add_control_reciever(control_reciever);

    main_loop->game_loop();
}
