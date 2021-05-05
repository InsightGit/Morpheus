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
            morpheus::utils::construct_appropriate_sprite_8bpp(true, main_loop->get_blending_controller(),
                                                                  main_loop->get_mosaic_controller(),
                                                                  false, false));

    main_loop->enable_affine(morpheus::core::gfx::AffineMode::MIXED_AFFINE);
    main_loop->get_blending_controller()->set_blending_mode(morpheus::core::gfx::BlendingMode::USE_WEIGHTS);

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                                        reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen, test8Pal,
                                        test8PalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32, 10);
    #elif _NDS
        static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sprite.get())->load_from_array(
                    reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen, test8Pal,
                    test8PalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32);
    #endif

    std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> animation_frames = get_animations(sprite.get());

    sprite->toggle_mosaic();

    sprite->set_frames(animation_frames);
    sprite->set_affine_index(0);

    #ifdef _NDS
        oamAffineTransformation(&oamMain, 0, 0x0100, 0x0, 0x0, 0x0100);
    #endif

    sprite->play();

    main_loop->add_sprite(sprite);

    main_loop->game_loop();
}
