//
// Created by bobby on 29/01/2021.
//

#include <nds/nds.hpp>

#include "custom1map.h"
#include "region_map2.h"

#pragma GCC diagnostic warning "-Wswitch"

class SwitchingBackgroundTestControls : public morpheus::utils::BackgroundTestControls {
public:
    SwitchingBackgroundTestControls(std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background,
                                    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background1) :
            morpheus::utils::BackgroundTestControls(background) {
        m_background = background;
        m_background1 = background1;
    }

    virtual void input(morpheus::core::InputEvent input_event) override {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            switch(input_event.button) {
                case morpheus::core::InputButton::A:
                    m_background->set_priority(1);

                    change_background(m_background1);
                    break;
                case morpheus::core::InputButton::B:
                    m_background->set_priority(0);

                    change_background(m_background);
                    break;
                default:
                    break;
            }

            scroll_background(input_event);
        }
    }

    virtual void update(unsigned char cycle_time) override {}
private:
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background1;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    /*morpheus::nds::gfx::TiledBackground4Bpp brin_test();
    morpheus::nds::gfx::TiledBackground8Bpp region_map();
    morpheus::nds::gfx::TiledBackground8Bpp custom_bg2();*/

    std::shared_ptr<morpheus::nds::gfx::TiledBackground8Bpp> custom_bg0(
            new morpheus::nds::gfx::TiledBackground8Bpp(
                        false, false, 0,
                        static_cast<morpheus::nds::gfx::NdsBlendingController*>(main_loop->get_blending_controller()),
                        static_cast<morpheus::nds::gfx::NdsMosaicController*>(main_loop->get_mosaic_controller()),
                        static_cast<morpheus::nds::NdsMainLoop*>(main_loop.get()),
                       1, 2));
    std::shared_ptr<morpheus::nds::gfx::TiledBackground8Bpp> custom_bg1(
            new morpheus::nds::gfx::TiledBackground8Bpp(
                    false, false, 1,
                    static_cast<morpheus::nds::gfx::NdsBlendingController*>(main_loop->get_blending_controller()),
                    static_cast<morpheus::nds::gfx::NdsMosaicController*>(main_loop->get_mosaic_controller()),
                    static_cast<morpheus::nds::NdsMainLoop*>(main_loop.get()),
                    3, 7));

    std::shared_ptr<SwitchingBackgroundTestControls> controls(new SwitchingBackgroundTestControls(custom_bg0,
                                                                                                  custom_bg1));

    defaultExceptionHandler();

    // Our CMake build file uses palette_id 10 for the map, so we use palette_id 10 here
    custom_bg0->load_from_array(custom1mapTiles, custom1mapTilesLen, custom1mapPal, custom1mapPalLen, 10,
                                custom1mapMap, custom1mapMapLen,
                               morpheus::core::gfx::TiledBackgroundSize::BG_64x64);
    // Our CMake build file uses palette_id 3 for the map, so we use palette_id 3 here
    custom_bg1->load_from_array(region_map2Tiles, region_map2TilesLen, region_map2Pal, region_map2PalLen, 3,
                                region_map2Map, region_map2MapLen,
                                morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    main_loop->add_control_reciever(controls);

    main_loop->game_loop();
}
