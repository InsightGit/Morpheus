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

#pragma GCC diagnostic ignored "-Wswitch"

// Grit Background includes
#include "maingamescreen.h"

#ifdef _NDS
#include "subscorescreen.h"
#endif

// Grit OBJ includes
#include "circlejewel.h"

#include "action_timer.hpp"
#include "soundbank.h"
#include "soundbank_bin.h"
#include "jewel.hpp"
#include "scene.hpp"

namespace puzzler {
    class MainGameScene : public puzzler::Scene, public morpheus::core::Node {
    public:
        MainGameScene(std::shared_ptr<morpheus::core::MainLoop> &main_loop);

        void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) override {
            if(m_active_jewel != nullptr) {
                m_active_jewel->draw(obj_attr_buffer, obj_attr_num, priority);
            }
        }
        void input(morpheus::core::InputEvent input_event)override;
        void setup()override;
        void update(unsigned char cycle_time)override;
    private:
        const morpheus::core::gfx::Vector2 MOVE_PER_SEC = morpheus::core::gfx::Vector2(10, 10);
        const int SCORE_TEXT_MAP_BASE = 22;
        const int SCORE_TEXT_TILE_BASE = 3;

        std::vector<unsigned int> get_gems_at_positions(std::vector<morpheus::core::gfx::Vector2> positions);
        bool is_gem_at_position(morpheus::core::gfx::Vector2 position) {
            std::vector<morpheus::core::gfx::Vector2> vector;

            vector.push_back(position);

            return is_gem_at_positions(vector);
        }
        bool is_gem_at_positions(std::vector<morpheus::core::gfx::Vector2> positions);

        #ifdef _NDS
            void setup_second_screen();
        #endif

        void update_gem_scoring(std::vector<JewelCollision> jewel_collision_results);

        std::unique_ptr<Jewel> m_active_jewel;
        std::unique_ptr<morpheus::core::audio::MaxModMusic> m_active_module;
        unsigned int m_cycles = 0;
        bool m_game_over = false;
        std::vector<std::unique_ptr<Jewel>> m_jewels;
        ActionTimer m_jewel_animation_timer;
        std::unique_ptr<morpheus::core::audio::MaxModSfx> m_jewel_complete_sfx;
        std::unique_ptr<morpheus::core::audio::MaxModSfx> m_jewel_put_sfx;
        ActionTimer m_jewel_spawning_timer;
        std::shared_ptr<morpheus::core::MainLoop> m_main_loop;
        unsigned int m_total_score = 0;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_user_background;

        #ifdef _NDS
            PrintConsole m_score_console;
            PrintConsole m_sub_console;
            std::unique_ptr<morpheus::nds::gfx::TiledBackground4Bpp> m_sub_background;
        #endif
    };
}

#endif //PUZZLER_MAIN_GAME_SCENE_HPP
