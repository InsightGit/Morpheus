//
// Created by bobby on 08/01/2021.
//

#include <core/core.hpp>

#include "region_map2.h"
#include "region_map.h"
#include "brin.h"

/*#ifdef _NDS
    #include <nds/nds.hpp>
    bool on_ds = true;
#elif _GBA
    #include <gba/gba.hpp>
    bool on_ds = false;
#else
    #error Unknown platform!
#endif*/

#include <gba/gba.hpp>
bool on_ds = false;

class Controls : public morpheus::core::Node {
public:
    void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority)override {}

    void input(morpheus::core::InputEvent input_event)override {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    m_y -= 10;
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    m_x -= 10;
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    m_x += 10;
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    m_y += 10;
                    break;
                default:
                    break;
            }

            REG_BG1HOFS = m_x;
            REG_BG1VOFS = m_y;
        }
    }
private:
    int m_x = 0;
    int m_y = 0;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(new morpheus::gba::GbaMainLoop(
                                                        morpheus::gba::DebugConsoleMode::OFF)/*morpheus::core::MainLoop::construct_appropriate_main_loop()*/);
    std::shared_ptr<Controls> controls(new Controls());

    if(on_ds) {
        //defaultExceptionHandler();
    } else {
        morpheus::gba::gfx::Background background(1,
                                                  std::static_pointer_cast<morpheus::gba::GbaMainLoop>(main_loop),
                                                  true,1, 0);

        /*background.load_from_array(reinterpret_cast<const unsigned int*>(brinTiles), brinTilesLen, brinPal, brinPalLen,
                                   brinMap, brinMapLen, morpheus::core::gfx::Vector2(64, 32));*/
        background.load_from_array(region_map2Tiles, region_map2TilesLen, region_map2Pal, region_map2PalLen,
                                   region_map2Map, region_map2MapLen, morpheus::core::gfx::Vector2(32, 32));
        /*background.load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                   region_mapMap, region_mapMapLen, morpheus::core::gfx::Vector2(32, 32));*/
    }

    main_loop->set_root(controls);

    main_loop->game_loop();
}

