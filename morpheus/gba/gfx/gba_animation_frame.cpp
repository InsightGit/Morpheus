//
// Created by bobby on 26/04/2021.
//

#include <gba/gfx/sprite.hpp>

#include "gba_animation_frame.hpp"

morpheus::gba::gfx::GbaAnimationFrame::GbaAnimationFrame(
                                                     Sprite *target_sprite, GbaAnimationFrame *from_animation_frame) :
                                                     core::gfx::AnimationFrame(target_sprite, from_animation_frame) {
    if(from_animation_frame != nullptr) {
        m_tile_id = from_animation_frame->m_tile_id;
    }
}

void morpheus::gba::gfx::GbaAnimationFrame::activate_on_target_sprite(core::gfx::AnimationFrameCopyOption copy_option) {
    auto *gba_sprite = static_cast<gba::gfx::Sprite*>(get_target_sprite());

    switch (static_cast<GbaAnimationFrameCopyOption>(copy_option)) {
        case GbaAnimationFrameCopyOption::TILE_ID:
            gba_sprite->build_attr2(gba_sprite->get_palette_id(), m_tile_id);
            break;
    }
}
