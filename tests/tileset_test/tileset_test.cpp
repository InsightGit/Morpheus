//
// Created by bobby on 08/01/2021.
//

#include <core/core.hpp>

#include "region_map2.h"
#include "region_map.h"
#include "brin.h"

#ifdef _NDS
    #include <nds/nds.hpp>
#elif _GBA
    #include <gba/gba.hpp>
#else
    #error Unknown platform!
#endif


class Controls : public morpheus::core::Node {
public:
    Controls(std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background) {
        m_background = background;
    }

    void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority)override {}

    void input(morpheus::core::InputEvent input_event)override {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            morpheus::core::gfx::Vector2 scroll_pos = m_background->get_scroll();

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

            m_background->set_scroll(scroll_pos);
        }
    }
private:
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::Utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background0;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background1;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background2;

    #ifdef _GBA
        background0.reset(new morpheus::gba::gfx::TiledBackground(1,
                                                                    std::static_pointer_cast<
                                                                        morpheus::gba::GbaMainLoop>(main_loop),
                                                                 true, 1, 0));
        background1.reset(new morpheus::gba::gfx::TiledBackground(2,
                                                              std::static_pointer_cast<
                                                                      morpheus::gba::GbaMainLoop>(main_loop),
                                                              true, 2, 1));
        /*background2.reset(new morpheus::gba::gfx::TiledBackground(3,
                                                                  std::static_pointer_cast<
                                                                          morpheus::gba::GbaMainLoop>(main_loop),
                                                                  false, 3, 2));*/
    #elif _NDS
        std::cout << "Initing TiledBackground class\n";

        defaultExceptionHandler();

        background0.reset(new morpheus::nds::gfx::TiledBackground(false, 1,
                                                                 std::static_pointer_cast<
                                                                    morpheus::nds::NdsMainLoop>(main_loop),
                                                            true,1, 1));
        background1.reset(new morpheus::nds::gfx::TiledBackground(false, 2,
                                                                  std::static_pointer_cast<
                                                                          morpheus::nds::NdsMainLoop>(main_loop),
                                                                  true, 2, 2));
        background2.reset(new morpheus::nds::gfx::TiledBackground(true, 2,
                                                                  std::static_pointer_cast<
                                                                          morpheus::nds::NdsMainLoop>(main_loop),
                                                                  false,2, 2));

        background2->load_from_array(reinterpret_cast<const unsigned int*>(brinTiles), brinTilesLen, brinPal, brinPalLen,
                                 brinMap, brinMapLen, morpheus::core::gfx::TiledBackgroundSize::BG_64x32);
    #endif

    std::shared_ptr<Controls> controls(new Controls(background0));

    std::cout << "Loading TiledBackground class\n";

    background0->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                           region_mapMap, region_mapMapLen * 4, morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
    background1->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                 region_mapMap, region_mapMapLen * 4,
                            morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    std::cout << "All ready!\n";

    main_loop->set_root(controls);

    main_loop->game_loop();
}
