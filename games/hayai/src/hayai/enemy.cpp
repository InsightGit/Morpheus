//
// Created by bobbyy on 5/23/21.
//

#include "enemy.h"

hayai::Enemy::Enemy(morpheus::core::gfx::BlendingController *blending_controller,
                    morpheus::core::gfx::MosaicController *mosaic_controller) {
    m_enemy_sprite.reset(
            morpheus::utils::construct_appropriate_sprite_8bpp(false, blending_controller, mosaic_controller));

}

void hayai::Enemy::update(const unsigned char cycle_time) {
    m_enemy_sprite->update(cycle_time);
}
