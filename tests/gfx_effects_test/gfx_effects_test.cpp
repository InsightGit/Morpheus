//
// Created by bobby on 22/03/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "region_map.h"
#include "test8.h"

class InputBase {
protected:
    morpheus::core::gfx::Vector2 input_routine(morpheus::core::InputEvent input_event,
                                               morpheus::core::gfx::Vector2 position) {
        if(input_event.state == morpheus::core::InputState::HELD || \
               input_event.state == morpheus::core::InputState::DOWN) {

            #ifdef _GBA
                nocash_puts("input detected");
            #elif _NDS
                nocashMessage("input detected");
            #endif

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() - 5);
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() + 5);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    position = morpheus::core::gfx::Vector2(position.get_x() - 5, position.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    position = morpheus::core::gfx::Vector2(position.get_x() + 5, position.get_y());
                    break;
                default:
                    break;
            }
        }

        return position;
    }
};

#ifdef _GBA
class MoveableSprite8 : public morpheus::gba::gfx::Sprite8Bpp, InputBase {
#elif _NDS
class MoveableSprite8 : public morpheus::nds::gfx::Sprite8Bpp, InputBase {
#endif
public:
    MoveableSprite8() :
    #ifdef _GBA
        morpheus::gba::gfx::Sprite8Bpp(nullptr) {}
    #elif _NDS
        morpheus::nds::gfx::Sprite8Bpp(true, nullptr) {}
    #endif
protected:
    void input(morpheus::core::InputEvent input_event)override {
        set_position(input_routine(input_event, get_position()));
    }
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite(
            morpheus::utils::construct_appropriate_sprite_8bpp(main_loop->get_blending_controller(), false, false));

    #ifdef _GBA
        background.reset(new morpheus::gba::gfx::TiledBackground(
                0, static_cast<morpheus::gba::gfx::GbaBlendingController*>(main_loop->get_blending_controller()),
                static_cast<morpheus::gba::GbaMainLoop*>(main_loop.get()), true, 2, 2));

        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                reinterpret_cast<const unsigned short *>(test8Pal), test8PalLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    #elif _NDS
    background.reset(new morpheus::gba::gfx::TiledBackground(0, main_loop->get_blending_controller(),
                                                                 main_loop.get(), true, 5, 5));

        static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                                                reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                                                reinterpret_cast<const unsigned short *>(test8Pal), test8PalLen,
                                                morpheus::core::gfx::SpriteSize::SIZE_32X32);
        nocashMessage("Loaded from array");
    #endif

    background->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                region_mapMap, region_mapMapLen, morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    background->set_mosaic_levels(morpheus::core::gfx::Vector2(10, 5));

    background->toggle_mosaic();

    sprite->set_position(50, 50);

    main_loop->set_root(sprite);

    main_loop->game_loop();
}
