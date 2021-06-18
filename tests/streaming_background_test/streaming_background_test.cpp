//
// Created by bobby on 15/06/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "region_map_128.h"

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

            m_background->set_global_scroll(scroll_pos);
        }
    }
private:
    std::shared_ptr<morpheus::core::gfx::StreamingBackgroundBase> m_background;
};

int main() {
    nocash_puts("Constructing MainLoop");

    auto main_loop = std::shared_ptr<morpheus::core::MainLoop>(morpheus::utils::construct_appropriate_main_loop());

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("Constructing TiledBackgroundBase");

    auto tiled_background_base = std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase>(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 0, nullptr, nullptr, main_loop.get(),
                                                                         3, 2, false));

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("Constructing StreamingBackgroundBase");

    auto streaming_background_base = std::shared_ptr<morpheus::core::gfx::StreamingBackgroundBase>(
            new morpheus::core::gfx::StreamingBackgroundBase(tiled_background_base.get(),
                                                             morpheus::core::gfx::Vector2(30, 20)));


    auto control_reciever = std::shared_ptr<StreamingBackgroundTestControls>(
            new StreamingBackgroundTestControls(streaming_background_base));

    main_loop->get_no_cash_debug_controller()->send_to_debug_window("Loading StreamingBackgroundBase");

    streaming_background_base->load_from_arrays(region_map_128Tiles, region_map_128TilesLen, region_map_128Pal,
                                                region_map_128PalLen, region_map_128MapsRows, region_map_128MapsLen,
                                                morpheus::core::gfx::StreamingBackgroundSize::BG_128x128);

    main_loop->add_control_reciever(control_reciever);

    main_loop->game_loop();
}
