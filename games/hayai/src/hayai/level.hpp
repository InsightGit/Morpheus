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
