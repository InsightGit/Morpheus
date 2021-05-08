//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_PLAYER_HPP
#define HAYAI_PLAYER_HPP

#include <array>

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#else
#error Unsupported platform!
#endif

#include "level.hpp"

#include "playerleftarms0.h"
#include "playerleftarms1.h"
#include "playerleftarms2.h"
#include "playerrightarms0.h"
#include "playerrightarms1.h"
#include "playerrightarms2.h"

namespace hayai {
    class Player : public morpheus::core::ControlReciever {
    public:
        Player(std::shared_ptr<morpheus::core::MainLoop> main_loop,
               std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> level_background,
               Level *level);

        morpheus::core::gfx::Vector2 get_velocity() const {
            return m_velocity;
        }

        std::shared_ptr<morpheus::core::gfx::SpriteBase> get_sprite() const {
            return m_sprite_base;
        }

        void draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num);

        void input(const morpheus::core::InputEvent input_event)override;
        void update(const unsigned char cycle_time)override;
    private:
        const std::array<unsigned int, 3> LEFT_PALETTE_IDS = {0, 1, 2};
        const std::array<unsigned int, 3> LEFT_TILE_IDS = {1, 21, 41};
        const std::array<unsigned int, 3> RIGHT_PALETTE_IDS = {3, 4, 5};
        const std::array<unsigned int, 3> RIGHT_TILE_IDS = {61, 81, 101};

        const unsigned int MAX_SPEED = 50;
        const int ACCELERATION_STEP = 2;

        void restrict_velocity(bool x, bool pos);
        void update_animation(bool left, unsigned int animation_frame);

        // explicit is better than implicit
        morpheus::core::gfx::Vector2 m_acceleration = morpheus::core::gfx::Vector2(0, 0);
        bool m_last_was_left = false;
        unsigned int m_current_animation_frame;
        Level *m_current_level;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_level_background;
        bool m_moved_this_frame = false;
        std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
        morpheus::core::gfx::Vector2 m_velocity = morpheus::core::gfx::Vector2(0, 8);
        morpheus::core::gfx::Vector2 m_past_velocity = morpheus::core::gfx::Vector2(0, 0);
    };
}

#endif //HAYAI_PLAYER_HPP
