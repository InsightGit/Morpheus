//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_PLAYER_HPP
#define HAYAI_PLAYER_HPP

#include <array>

#ifdef _GBA
#include <morpheus/gba/gba.hpp>
#elif _NDS
#include <morpheus/nds/nds.hpp>
#else
#error Unsupported platform!
#endif

#include "player_hud.hpp"
#include "level.hpp"

#include "playerleftarms0.h"
#include "playerleftarms1.h"
#include "playerleftarms2.h"
#include "playerrightarms0.h"
#include "playerrightarms1.h"
#include "playerrightarms2.h"

#include "soundbank.h"
#include "soundbank_bin.h"

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

        bool is_game_over() const {
            return m_game_over;
        }

        void set_enemies_vector(const std::vector<std::shared_ptr<Enemy>> &enemies) {
            m_enemies = enemies;
        }

        void draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num);

        void input(const morpheus::core::InputEvent input_event)override;
        void update(const unsigned char cycle_time)override;
    private:
        const std::array<unsigned int, 3> LEFT_PALETTE_IDS = {0, 1, 2};
        const std::array<unsigned int, 3> LEFT_TILE_IDS = {1, 21, 41};
        const std::array<unsigned int, 3> RIGHT_PALETTE_IDS = {3, 4, 5};
        const std::array<unsigned int, 3> RIGHT_TILE_IDS = {61, 81, 101};

        const int ACCELERATION_STEP = 6;
        const bool ENABLE_ACCEL_MOVEMENT_SYSTEM = false;
        const int FLICKER_LENGTH = 240;
        const int FRICTION = 2;
        const int GAME_OVER_FADE_TIME_FRAMES = 60;
        const int GRAVITY = 4;
        const morpheus::core::gfx::Vector2 INITIAL_SCROLL_POSITION = morpheus::core::gfx::Vector2(32*8, 30*8);
        const morpheus::core::gfx::Vector2 INITIAL_SPRITE_POSITION = morpheus::core::gfx::Vector2(88, 88);
        const int JUMPING_ASCENT_TIME_FRAMES = 30;
        const int JUMPING_SPEED = 6;
        const unsigned int MAX_SPEED = 7;
        const morpheus::core::gfx::Vector2 PLAYER_SIZE = morpheus::core::gfx::Vector2(32, 32);
        const int REGULAR_SPEED = 4;
        const int SPEED_ZONE_ACCEL = 3;

        enum class SpeedZone {
            NO_EFFECT,
            SPEED_LEFT,
            SPEED_RIGHT
        };

        bool is_player_collided_with(morpheus::core::gfx::Vector2 point1, morpheus::core::gfx::Vector2 point2) const {
            return point1.get_x() >= point2.get_x() &&
                   point1.get_y() >= point2.get_y() &&
                   point1.get_x() < point2.get_x() + PLAYER_SIZE.get_x() &&
                   point1.get_y() < point2.get_y() + PLAYER_SIZE.get_y();
        }

        void apply_enemy_collision_detection();
        void apply_friction();
        void apply_gravity();
        void apply_speed_zones();
        void apply_x_collision_detection();
        void apply_y_collision_detection();

        void remove_coin_at_position(morpheus::core::gfx::Vector2 position);
        //void update_coin_count();

        bool coin_tile_index(unsigned int tile_index);
        bool collision_tile_id(unsigned int tile_id);
        bool friction_tile_id(unsigned int tile_id);
        SpeedZone get_speed_zone() const;
        void update_animation(bool left, unsigned int animation_frame);

        // explicit is better than implicit
        morpheus::core::gfx::Vector2 m_acceleration = morpheus::core::gfx::Vector2(0, 0);
        std::unique_ptr<morpheus::core::audio::MaxModSfx> m_coin_pickup_sfx;
        unsigned int m_current_animation_frame;
        Level *m_current_level;
        std::vector<std::shared_ptr<Enemy>> m_enemies;
        std::unique_ptr<morpheus::core::audio::MaxModSfx> m_enemy_damage_sfx;
        bool m_first_run = true;
        std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> m_flicker_animation;
        std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> m_game_over_animation;
        bool m_game_over = false;
        bool m_game_over_printed = false;
        std::unique_ptr<morpheus::core::gfx::TextBase> m_game_over_text;
        std::unique_ptr<morpheus::core::gfx::Window> m_game_over_text_window;
        std::vector<uint16_t*> m_gfx_pointers;
        bool m_last_was_left = false;
        bool m_jumping = false;
        int m_jumping_frame = 0;
        int m_jumping_update_frame = 0;
        bool m_left = false;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_level_background;
        bool m_moved_this_frame = false;
        std::unique_ptr<morpheus::core::audio::MaxModSfx> m_player_damage_sfx;
        std::unique_ptr<PlayerHud> m_player_hud;
        std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
        int m_target_x_velocity = 0;
        int m_target_y_velocity = 0;
        morpheus::core::gfx::Vector2 m_velocity = morpheus::core::gfx::Vector2(0, 8);
        morpheus::core::gfx::Vector2 m_past_velocity = morpheus::core::gfx::Vector2(0, 0);
    };
}

#endif //HAYAI_PLAYER_HPP
