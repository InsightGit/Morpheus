//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_LEVEL_HPP
#define HAYAI_LEVEL_HPP

#include "scene.hpp"

#include "test_map_1.h"

namespace hayai {
    class Player;

    class Level : public Scene {
    public:
        static const std::vector<unsigned int> BLOCK_TILES;
        static const std::vector<unsigned int> DARK_COIN_TILES;
        static const std::vector<unsigned int> DIRT_TILES;
        static const std::vector<unsigned int> GRASS_TILES;
        static const std::vector<unsigned int> ICE_TILES;
        static const std::vector<unsigned int> LIT_COIN_TILES;
        static const std::vector<unsigned int> QUESTION_BLOCK_TILES;
        static const std::vector<unsigned int> SLUSH_TILES;
        static const std::vector<unsigned int> SPEED_DOWN_TILES;
        static const std::vector<unsigned int> SPEED_UP_TILES;
        static const std::vector<unsigned int> YELLOW_BLOCK_TILES;

        static const std::vector<std::vector<unsigned int>> COLLISION_TILES;

        Level(std::shared_ptr<morpheus::core::MainLoop> main_loop);

        ~Level() override = default;

        std::vector<unsigned int> get_collision_tile_ids() const {
            return m_collision_tile_ids;
        }

        void input(const morpheus::core::InputEvent input_event)override;
        void update(const unsigned char cycle_time)override;
    private:
        std::vector<unsigned int> m_collision_tile_ids;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_level_background;
        std::unique_ptr<Player> m_player;
    };
}

#endif //HAYAI_LEVEL_HPP
