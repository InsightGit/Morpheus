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

const unsigned int hayai::Level::MAIN_LEVEL_BACKGROUND_NUM = 2;

hayai::Level::Level(std::shared_ptr<morpheus::core::MainLoop> main_loop) : Scene(main_loop) {
    m_level_background.reset(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, MAIN_LEVEL_BACKGROUND_NUM,
                                                                         main_loop->get_blending_controller(),
                                                                         main_loop->get_mosaic_controller(),
                                                                         main_loop.get(), 3, 1));

    m_player = std::make_unique<Player>(main_loop, m_level_background, this);
    m_enemies.push_back(std::make_unique<Enemy>(main_loop->get_blending_controller(),
                                                main_loop->get_mosaic_controller(),
                                                main_loop->get_no_cash_debug_controller(),
                                                m_level_background,
                                                morpheus::core::gfx::Vector2(16 * 8, 42 * 8),
                                                morpheus::core::gfx::Vector2(22 * 8, 42 * 8),
                                                m_player->get_sprite()));

    m_level_background->load_from_array(test_map_1Tiles, test_map_1TilesLen, test_map_1Pal,
                                        test_map_1PalLen, test_map_1Map, test_map_1MapLen,
                                        morpheus::core::gfx::TiledBackgroundSize::BG_64x64);
    m_player->set_enemies_vector(m_enemies);

    for(const std::vector<unsigned int> &collision_tile_group : COLLISION_TILES) {
        m_collision_tile_ids.insert(m_collision_tile_ids.end(), collision_tile_group.begin(),
                                    collision_tile_group.end());
    }

    for(const std::vector<unsigned int> &friction_tile_group : FRICTION_TILES) {
        m_friction_tile_ids.insert(m_friction_tile_ids.end(), friction_tile_group.begin(),
                                   friction_tile_group.end());
    }

    for(int i = 0; test_map_1MapCoinsTileIndexLen > i; ++i) {
        m_current_coin_indices.push_back(test_map_1MapCoinsTileIndex[i]);
    }

    main_loop->add_sprite(m_player->get_sprite());

    for(std::shared_ptr<Enemy> &enemy : m_enemies) {
        main_loop->add_sprite(enemy->get_sprite());
    }

    m_old_scroll_position = m_level_background->get_scroll();
}

void hayai::Level::input(const morpheus::core::InputEvent input_event) {
    m_player->input(input_event);
}

void hayai::Level::update(const unsigned char cycle_time) {
    //int y_scroll_difference;

    m_player->update(cycle_time);

    for(std::shared_ptr<Enemy> &enemy : m_enemies) {
        enemy->update(cycle_time);
    }

    #ifdef _NDS
    if(cycle_time == 30 || cycle_time == 0) {
        animate_coins();
    }
    #endif

    m_level_background->set_scroll(m_level_background->get_scroll() + m_player->get_velocity());
}

void hayai::Level::animate_coins() {
    for(unsigned int i = 0; m_current_coin_indices.size() > i; ++i) {
        int tile_id = m_level_background->get_tile_id_at_index(m_current_coin_indices[i]);

        for(unsigned int i2 = 0; DARK_COIN_TILES.size() > i2; ++i2) {
            if(static_cast<unsigned int>(tile_id) == DARK_COIN_TILES[i2]) {
                m_level_background->set_tile_id_at_index(m_current_coin_indices[i], LIT_COIN_TILES[i2]);
                break;
            }
        }

        for(unsigned int i2 = 0; LIT_COIN_TILES.size() > i2; ++i2) {
            if(static_cast<unsigned int>(tile_id) == LIT_COIN_TILES[i2]) {
                m_level_background->set_tile_id_at_index(m_current_coin_indices[i], DARK_COIN_TILES[i2]);
                break;
            }
        }
    }
}

void hayai::Level::delete_coin_indices(const std::array<int, 4> coin_indices) {
    for(int coin_index : coin_indices) {
        for(unsigned int i = 0; m_current_coin_indices.size() > i; ++i) {
            if(coin_index == m_current_coin_indices[i]) {
                m_current_coin_indices.erase(m_current_coin_indices.begin() + static_cast<int>(i));
                m_level_background->set_tile_id_at_index(coin_index, 0x0);

                break;
            }
        }
    }
}

void hayai::Level::kill_enemy(const std::shared_ptr<Enemy> &enemy) {
    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
                   [this, enemy](std::shared_ptr<Enemy> array_enemy) {
       for(unsigned int i = 0; i < m_enemies.size(); ++i) {
           if(enemy.get() == m_enemies[i].get()) {
               enemy->get_sprite()->hide();

               get_main_loop()->remove_sprite(enemy->get_sprite());

               return true;
           }
       }

       return false;
    }), m_enemies.end());
}
