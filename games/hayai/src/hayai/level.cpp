//
// Created by bobby on 04/05/2021.
//

#include "level.hpp"

hayai::Level::Level(std::shared_ptr<morpheus::core::MainLoop> main_loop) : Scene(main_loop) {
    m_level_background.reset(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 2,
                                                                         main_loop->get_blending_controller(),
                                                                         main_loop->get_mosaic_controller(),
                                                                         main_loop.get(), 10, 10));

    m_level_background->load_from_array(test_map_1Tiles, test_map_1TilesLen, test_map_1Pal, test_map_1PalLen,
                                        test_map_1Map, test_map_1MapLen,
                                        morpheus::core::gfx::TiledBackgroundSize::BG_64x64);
}
