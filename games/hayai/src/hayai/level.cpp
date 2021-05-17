//
// Created by bobby on 04/05/2021.
//

#include "level.hpp"

#include "player.hpp"

#include <memory>

const std::vector<unsigned int> hayai::Level::BLOCK_TILES = {0x09, 0x0A, 0x19, 0x1A};
const std::vector<unsigned int> hayai::Level::DARK_COIN_TILES = {0x22, 0x23, 0x32, 0x33};
const std::vector<unsigned int> hayai::Level::DIRT_TILES = {0x03, 0x04, 0x13, 0x14};
const std::vector<unsigned int> hayai::Level::GRASS_TILES = {0x01, 0x02, 0x11, 0x12};
const std::vector<unsigned int> hayai::Level::ICE_TILES = {0x07, 0x08, 0x17, 0x18};
const std::vector<unsigned int> hayai::Level::INVISIBLE_WALL_TILES = {0x10};
const std::vector<unsigned int> hayai::Level::LIT_COIN_TILES = {0x24, 0x25, 0x34, 0x35};
const std::vector<unsigned int> hayai::Level::QUESTION_BLOCK_TILES = {0x05, 0x06, 0x15, 0x16};
const std::vector<unsigned int> hayai::Level::SLUSH_TILES = {0x20, 0x21, 0x30, 0x31};
const std::vector<unsigned int> hayai::Level::SPEED_LEFT_TILES = {0x1D, 0x1E};
const std::vector<unsigned int> hayai::Level::SPEED_RIGHT_TILES = {0x0D, 0x0E};
const std::vector<unsigned int> hayai::Level::YELLOW_BLOCK_TILES = {0x0B, 0x0C, 0x1B, 0x1C};

const std::vector<std::vector<unsigned int>> hayai::Level::COLLISION_TILES = {BLOCK_TILES, DIRT_TILES, GRASS_TILES,
                                                                              ICE_TILES, INVISIBLE_WALL_TILES,
                                                                              QUESTION_BLOCK_TILES, SLUSH_TILES,
                                                                              YELLOW_BLOCK_TILES};
const std::vector<std::vector<unsigned int>> hayai::Level::FRICTION_TILES = {BLOCK_TILES, DIRT_TILES, GRASS_TILES,
                                                                             INVISIBLE_WALL_TILES, SLUSH_TILES,
                                                                             YELLOW_BLOCK_TILES};

hayai::Level::Level(std::shared_ptr<morpheus::core::MainLoop> main_loop) : Scene(main_loop) {
    m_level_background.reset(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 2,
                                                                         main_loop->get_blending_controller(),
                                                                         main_loop->get_mosaic_controller(),
                                                                         main_loop.get(), 2, 1));
    m_player = std::make_unique<Player>(main_loop, m_level_background, this);

    m_level_background->load_from_array(test_map_1Tiles, test_map_1TilesLen, test_map_1Pal,
                                        test_map_1PalLen, test_map_1Map, test_map_1MapLen,
                                        morpheus::core::gfx::TiledBackgroundSize::BG_64x64);

    for(const std::vector<unsigned int> &collision_tile_group : COLLISION_TILES) {
        m_collision_tile_ids.insert(m_collision_tile_ids.end(), collision_tile_group.begin(),
                                    collision_tile_group.end());
    }

    for(const std::vector<unsigned int> &friction_tile_group : FRICTION_TILES) {
        m_friction_tile_ids.insert(m_friction_tile_ids.end(), friction_tile_group.begin(),
                                   friction_tile_group.end());
    }

    main_loop->add_sprite(m_player->get_sprite());
}

void hayai::Level::input(const morpheus::core::InputEvent input_event) {
    m_player->input(input_event);
}

void hayai::Level::update(const unsigned char cycle_time) {
    m_player->update(cycle_time);

    //nocash_message("velocity was " + m_player->get_velocity().to_string());

    m_level_background->set_scroll(m_level_background->get_scroll() + m_player->get_velocity());
}
