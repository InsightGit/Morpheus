//
// Created by bobby on 04/05/2021.
//

#include "level.hpp"

#include "player.hpp"

#include <memory>

hayai::Level::Level(std::shared_ptr<morpheus::core::MainLoop> main_loop) : Scene(main_loop) {
    m_level_background.reset(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 2,
                                                                         main_loop->get_blending_controller(),
                                                                         main_loop->get_mosaic_controller(),
                                                                         main_loop.get(), 2, 1));
    m_player = std::make_unique<Player>(main_loop, m_level_background, this);

    m_level_background->load_from_array(test_map_1Tiles, test_map_1TilesLen, test_map_1Pal, test_map_1PalLen,
                                        test_map_1Map, test_map_1MapLen,
                                        morpheus::core::gfx::TiledBackgroundSize::BG_64x64);

    for(int i = 1; 10 > i; ++i) {
        m_collision_tile_ids.push_back(i);
    }

    main_loop->add_sprite(m_player->get_sprite());
}

void hayai::Level::input(const morpheus::core::InputEvent input_event) {
    m_player->input(input_event);
}

void hayai::Level::update(const unsigned char cycle_time) {
    m_level_background->set_scroll(m_level_background->get_scroll() + m_player->get_velocity());

    m_player->update(cycle_time);
}
