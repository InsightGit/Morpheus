//
// Created by bobbyy on 5/23/21.
//

#include "enemy.hpp"

const morpheus::core::gfx::Vector2 hayai::Enemy::ENEMY_SIZE = morpheus::core::gfx::Vector2(32, 25);
const morpheus::core::gfx::Vector2 hayai::Enemy::JUMPING_OFFSET = morpheus::core::gfx::Vector2(0, 2);
const morpheus::core::gfx::Vector2 hayai::Enemy::REGULAR_OFFSET = morpheus::core::gfx::Vector2(0, 7);

hayai::Enemy::Enemy(morpheus::core::gfx::BlendingController *blending_controller,
                    morpheus::core::gfx::MosaicController *mosaic_controller,
                    morpheus::core::NoCashDebugController *debug_controller,
                    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> level_background,
                    morpheus::core::gfx::Vector2 initial_map_position,
                    morpheus::core::gfx::Vector2 final_map_position,
                    std::shared_ptr<morpheus::core::gfx::SpriteBase> player_sprite) {
    m_debug_controller = debug_controller;
    // TODO(Bobby): Find a way to get rid of these needed 11 tile offsets
    m_final_map_position = final_map_position + morpheus::core::gfx::Vector2(11 * 8, 11 * 8);
    m_initial_map_position = initial_map_position + morpheus::core::gfx::Vector2(11 * 8, 11 * 8);
    m_level_background = level_background;
    m_player_sprite = player_sprite;

    m_enemy_sprite.reset(
            morpheus::utils::construct_appropriate_sprite_4bpp(false, blending_controller, mosaic_controller));
    m_enemy_animation_model_sprite.reset(morpheus::utils::construct_appropriate_sprite_4bpp(false, nullptr, nullptr));

    #ifdef _GBA
        auto *gba_sprite = static_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_enemy_sprite.get());

        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(enemydownTiles),
                                    enemydownTilesLen, 7, morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    ENEMY_TILE_ID);
        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(enemyupTiles),
                                    enemyupTilesLen, 7, morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    ENEMY_JUMPING_TILE_ID);
    #elif _NDS
    #endif

    m_enemy_sprite->load_into_palette(enemydownPal, 16, ENEMY_PALETTE_ID * 16);

    m_current_animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(m_enemy_animation_model_sprite.get())));

    m_current_animation_frames.back()->set_position(m_initial_map_position, true,
                                                    morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    m_current_animation_frames.back()->set_vblank_delays(120);

    m_current_animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(m_enemy_animation_model_sprite.get())));

    m_current_animation_frames.back()->set_position(m_final_map_position, true,
                                                    morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    m_current_animation_frames.back()->set_vblank_delays(120);

    m_enemy_animation_model_sprite->set_frames(m_current_animation_frames);

    m_enemy_animation_model_sprite->play();

    //m_enemy_sprite->set_frames(m_current_animation_frames);

    m_debug_controller->send_to_debug_window("animation size is " + std::to_string(m_current_animation_frames.size()));

    //m_enemy_sprite->play();
}

void hayai::Enemy::update(const unsigned char cycle_time) {
    if(cycle_time == 0 || cycle_time == 15 || cycle_time == 30 || cycle_time == 45) {
        #ifdef _GBA
            auto *gba_sprite = static_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_enemy_sprite.get());

            m_jumping = !m_jumping;

            if(m_jumping) {
                gba_sprite->build_attr2(ENEMY_PALETTE_ID, ENEMY_JUMPING_TILE_ID);
            } else {
                gba_sprite->build_attr2(ENEMY_PALETTE_ID, ENEMY_TILE_ID);
            }
        #elif _NDS
        #endif
    }

    m_enemy_sprite->update(cycle_time);
    m_enemy_animation_model_sprite->update(cycle_time);

    update_animation_positions();
}

void hayai::Enemy::update_animation_positions() {
    morpheus::core::gfx::Vector2 enemy_map_position = m_enemy_animation_model_sprite->get_position();
    morpheus::core::gfx::Vector2 player_map_position = m_player_sprite->get_position() + m_level_background->get_scroll();
    auto sprite_map_position_delta = morpheus::core::gfx::Vector2(enemy_map_position.get_x() - player_map_position.get_x(),
                                                                  enemy_map_position.get_y() - player_map_position.get_y());

    if(abs(sprite_map_position_delta.get_x()) < 240 && abs(sprite_map_position_delta.get_y()) < 160) {
        m_enemy_sprite->set_position(sprite_map_position_delta);

        m_enemy_sprite->show();
    } else {
        m_enemy_sprite->hide();
    }
}
