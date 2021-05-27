//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_LEVEL_HPP
#define HAYAI_LEVEL_HPP

#include "enemy.hpp"
#include "scene.hpp"

#include "test_map_1.h"
#include "test_map_1_128.h"

namespace hayai {
    class Player;

    class Level : public Scene {
    public:
        static const std::vector<unsigned int> BLOCK_TILES;
        static const std::vector<unsigned int> DARK_COIN_TILES;
        static const std::vector<unsigned int> DIRT_TILES;
        static const std::vector<unsigned int> GRASS_TILES;
        static const std::vector<unsigned int> ICE_TILES;
        static const std::vector<unsigned int> INVISIBLE_WALL_TILES;
        static const std::vector<unsigned int> LIT_COIN_TILES;
        static const std::vector<unsigned int> QUESTION_BLOCK_TILES;
        static const std::vector<unsigned int> SLUSH_TILES;
        static const std::vector<unsigned int> SPEED_LEFT_TILES;
        static const std::vector<unsigned int> SPEED_RIGHT_TILES;
        static const std::vector<unsigned int> YELLOW_BLOCK_TILES;

        static const std::vector<std::vector<unsigned int>> COLLISION_TILES;
        static const std::vector<std::vector<unsigned int>> FRICTION_TILES;

        static const unsigned int MAIN_LEVEL_BACKGROUND_NUM;

        Level(std::shared_ptr<morpheus::core::MainLoop> main_loop);

        ~Level() override = default;

        std::vector<unsigned int> get_collision_tile_ids() const {
            return m_collision_tile_ids;
        }

        std::vector<int> get_current_coin_indices() const {
            return m_current_coin_indices;
        }

        std::vector<unsigned int> get_friction_tile_ids() const {
            return m_friction_tile_ids;
        }

        void nocash_message(std::string message) {
            get_main_loop()->get_no_cash_debug_controller()->send_to_debug_window(message);
        }

        void delete_coin_indices(std::array<int, 4> coin_indices);

        void input(const morpheus::core::InputEvent input_event)override;
        void update(const unsigned char cycle_time)override;
    private:
        void animate_coins();

        std::vector<int> m_coin_indices_to_delete;
        std::vector<unsigned int> m_collision_tile_ids;
        std::vector<int> m_current_coin_indices;
        int m_current_top_tile_row_to_load = 64;
        int m_current_bottom_tile_row_to_load = 0;
        std::vector<std::shared_ptr<Enemy>> m_enemies;
        std::unique_ptr<Player> m_player;
        std::vector<unsigned int> m_friction_tile_ids;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_level_background;
        morpheus::core::gfx::Vector2 m_old_scroll_position;
    };
}

#endif //HAYAI_LEVEL_HPP
