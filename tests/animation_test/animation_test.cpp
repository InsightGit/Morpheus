//
// Created by bobby on 26/04/2021.
//

#ifdef _GBA
    #include <gba/gba.hpp>
#elif _NDS
    #include <nds/nds.hpp>
#endif

#include "test_animation.hpp"
#include "test8.h"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sprite(
            morpheus::utils::construct_appropriate_sprite_8bpp(false, main_loop->get_blending_controller(),
                                                                  main_loop->get_mosaic_controller(),
                                                                  false, false));

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                                        reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen, test8Pal,
                                        test8PalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32, 10);
    #elif _NDS
    #endif

    std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> animation_frames = get_animations(sprite.get());

    sprite->toggle_mosaic();

    sprite->set_frames(animation_frames);

    sprite->play();

    main_loop->add_sprite(sprite);

    main_loop->game_loop();
}
