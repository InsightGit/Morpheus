//
// Created by bobbyy on 5/23/21.
//

#ifndef HAYAI_ENEMY_HPP
#define HAYAI_ENEMY_HPP

#ifdef _GBA
    #include <gba/gba.hpp>
#elif _NDS
    #include <nds/nds.hpp>
#endif

#include "enemydown.h"
#include "enemyup.h"

namespace hayai {
    class Enemy : public morpheus::core::ControlReciever {
    public:
        static const morpheus::core::gfx::Vector2 ENEMY_SIZE;
        static const morpheus::core::gfx::Vector2 JUMPING_OFFSET;
        static const morpheus::core::gfx::Vector2 REGULAR_OFFSET;

        Enemy(morpheus::core::gfx::BlendingController *blending_controller,
              morpheus::core::gfx::MosaicController *mosaic_controller,
              morpheus::core::NoCashDebugController *debug_controller,
              std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> level_background,
              morpheus::core::gfx::Vector2 final_map_position,
              morpheus::core::gfx::Vector2 initial_map_position,
              std::shared_ptr<morpheus::core::gfx::SpriteBase> player_sprite);

        ~Enemy() override = default;

        std::shared_ptr<morpheus::core::gfx::SpriteBase> get_sprite() const {
            return m_enemy_sprite;
        }

        morpheus::core::gfx::Vector2 get_enemy_map_position() const {
            return m_enemy_animation_model_sprite->get_position();
        }

        bool is_jumping() const {
            return m_jumping;
        }

        void input(const morpheus::core::InputEvent input_event) override {}

        void update(const unsigned char cycle_time)override;
    private:
        static const unsigned int ENEMY_PALETTE_ID = 7;
        static const unsigned int ENEMY_TILE_ID = 121;
        static const unsigned int ENEMY_JUMPING_TILE_ID = 141;

        void update_animation_positions();

        std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> m_current_animation_frames;
        morpheus::core::NoCashDebugController *m_debug_controller;
        std::shared_ptr<morpheus::core::gfx::SpriteBase> m_enemy_animation_model_sprite;
        std::shared_ptr<morpheus::core::gfx::SpriteBase> m_enemy_sprite;
        morpheus::core::gfx::Vector2 m_final_map_position;
        morpheus::core::gfx::Vector2 m_initial_map_position;
        bool m_jumping = false;
        uint16_t *m_jumping_gfx_pointer;
        std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_level_background;
        std::shared_ptr<morpheus::core::gfx::SpriteBase> m_player_sprite;
        uint16_t *m_down_gfx_pointer;
    };
}

#endif //HAYAI_ENEMY_HPP
