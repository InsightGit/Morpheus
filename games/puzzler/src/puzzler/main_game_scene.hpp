//
// Created by bobby on 07/02/2021.
//

#ifndef PUZZLER_MAIN_GAME_SCENE_HPP
#define PUZZLER_MAIN_GAME_SCENE_HPP

#include <vector>

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#pragma GCC diagnostic warning "-Wswitch"

// Grit Background includes
#include "maingamescreen.h"

// Grit OBJ includes
#include "circlejewel.h"

#include "jewel.hpp"
#include "scene.hpp"

namespace puzzler {
    class MainGameScene : public puzzler::Scene, public morpheus::core::Node {
    public:
        MainGameScene(std::shared_ptr<morpheus::core::MainLoop> &main_loop);

        void draw_node(std::vector<void *> obj_attr_buffer, int obj_attr_num, int priority) override {
            if(m_active_jewel != nullptr) {
                m_active_jewel->draw_node(obj_attr_buffer, obj_attr_num, priority);
            }
        }
        void input(morpheus::core::InputEvent input_event)override;
        void setup()override;
        void update(unsigned char cycle_time)override;
    private:
        const morpheus::core::gfx::Vector2 MOVE_PER_SEC = morpheus::core::gfx::Vector2(10, 10);
        const int SCORE_TEXT_MAP_BASE = 22;
        const int SCORE_TEXT_TILE_BASE = 3;

        std::unique_ptr<Jewel> m_active_jewel;
        unsigned char m_current_action_cycle;
        unsigned int m_cycles = 0;
        bool m_current_action_cycle_waiting = false;
        std::vector<std::unique_ptr<Jewel>> m_jewels;
        std::shared_ptr<morpheus::core::MainLoop> m_main_loop;
        unsigned int m_total_score = 0;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_user_background;

        #ifdef _NDS
            PrintConsole m_score_console;
        #endif

        std::vector<unsigned int> get_gems_at_positions(std::vector<morpheus::core::gfx::Vector2> positions);
        bool is_gem_at_position(morpheus::core::gfx::Vector2 position) {
            std::vector<morpheus::core::gfx::Vector2> vector;

            vector.push_back(position);

            return is_gem_at_positions(vector);
        }
        bool is_gem_at_positions(std::vector<morpheus::core::gfx::Vector2> positions);

        void update_gem_scoring(std::vector<JewelCollision> jewel_collision_results);
    };
}

#endif //PUZZLER_MAIN_GAME_SCENE_HPP
