#ifdef _GBA
    #include <morpheus/gba/gba.hpp>
#elif _NDS
    #include <morpheus/nds/nds.hpp>
#else
    #error Unsupported platform!
#endif


#include "sprite_4bpp.h"
#include "sprite_8bpp.h"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TextBase> hello_world_text(morpheus::utils::construct_appropriate_text(
                                                                                      false, 0, 0, 0, main_loop.get()));
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> sample_background(morpheus::utils::
            construct_appropriate_tiled_background_4bpp(false, 1, main_loop->get_blending_controller(),
                                                        main_loop->get_mosaic_controller(), main_loop.get(), 1, 5));
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sample_sprite(morpheus::utils::construct_appropriate_sprite_8bpp(
            false, main_loop->get_blending_controller(), main_loop->get_mosaic_controller()));
    /* or if you want a 4bpp sprite instead:
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sample_sprite(morpheus::utils::construct_appropriate_sprite_4bpp(
            false, main_loop->get_blending_controller(), main_loop->get_mosaic_controller()));*/

    #ifdef _GBA
        auto gba_sprite = static_cast<morpheus::gba::gfx::Sprite*>(sample_sprite.get());

        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(sprite_8bppTiles), sprite_8bppTilesLen,
                                    sprite_8bppPal, sprite_8bppPalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    #elif _NDS
        auto nds_sprite = static_cast<morpheus::nds::gfx::Sprite*>(sample_sprite.get());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(sprite_8bppTiles), sprite_8bppTilesLen,
                                    sprite_8bppPal, sprite_8bppPalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    #endif

    main_loop->add_sprite(sample_sprite);

    hello_world_text.print_at_pos("Hello World!", morpheus::core::gfx::Vector2(0, 0));

    main_loop->game_loop();
}
