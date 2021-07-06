#ifdef _GBA
    #include <morpheus/gba/gba.hpp>
#elif _NDS
    #include <morpheus/nds/nds.hpp>
#endif


int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TextBase> hello_world_text(morpheus::utils::construct_appropriate_text(
                                                                                      false, 0, 0, 0, main_loop.get()));
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> sample_background(morpheus::utils::
            construct_appropriate_tiled_background_4bpp(false, 1, main_loop->get_blending_controller(),
                                                        main_loop->get_mosaic_controller(), main_loop.get(), 1, 5));
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sample_sprite(morpheus::utils::construct_appropriate_sprite_4bpp(
            false, main_loop->get_blending_controller(), main_loop->get_mosaic_controller()));

    hello_world_text.print_at_pos("Hello World!", morpheus::core::gfx::Vector2(0, 0));

    main_loop->game_loop();
}
