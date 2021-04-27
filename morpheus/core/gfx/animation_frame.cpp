//
// Created by bobby on 26/04/2021.
//

#include "animation_frame.hpp"

morpheus::core::gfx::AnimationFrame::AnimationFrame(core::gfx::SpriteBase *sprite_base,
                                                    AnimationFrame *from_animation_frame) {
    m_target_sprite = sprite_base;

    if(from_animation_frame != nullptr) {
        m_animation_frame_copy_options = from_animation_frame->m_animation_frame_copy_options;
        m_blending_value = from_animation_frame->m_blending_value;
        m_mosaic_levels = from_animation_frame->m_mosaic_levels;
        m_palette_id = from_animation_frame->m_palette_id;
        m_position = from_animation_frame->m_position;
        m_rotation = from_animation_frame->m_rotation;
        m_scale = from_animation_frame->m_scale;
        m_sprite_size = from_animation_frame->m_sprite_size;
        m_target_sprite = from_animation_frame->m_target_sprite;
        m_visible = from_animation_frame->m_visible;
    }
}

void morpheus::core::gfx::AnimationFrame::activate_on_target_sprite_base() {
    core::gfx::BlendingSetting blending_setting = m_target_sprite->get_blending_controller()->
                                                  get_object_blending_setting();

    for(AnimationFrameCopyOption copy_option : m_animation_frame_copy_options) {
        switch (copy_option) {
            case AnimationFrameCopyOption::BLENDING:
                if(blending_setting != core::gfx::BlendingSetting::OFF) {
                    switch (m_target_sprite->get_blending_controller()->get_blending_mode()) {
                        case core::gfx::BlendingMode::FADE_TO_WHITE:
                        case core::gfx::BlendingMode::FADE_TO_BLACK:
                            m_target_sprite->get_blending_controller()->set_blend_fade(m_blending_value);
                            break;
                        case core::gfx::BlendingMode::USE_WEIGHTS:
                            m_target_sprite->get_blending_controller()->set_blend_weight(
                                    blending_setting == core::gfx::BlendingSetting::BOTTOM_ON,
                                    m_blending_value);
                            break;
                        case core::gfx::BlendingMode::OFF:
                            break;
                    }
                }
                break;
            case AnimationFrameCopyOption::SPRITE_SIZE:
                m_target_sprite->set_sprite_size(m_sprite_size);
                break;
            case AnimationFrameCopyOption::MOSAIC:
                m_target_sprite->set_mosaic_levels(m_mosaic_levels);
                break;
            case AnimationFrameCopyOption::PALETTE:
                // TODO(Bobby): implement this ree
                break;
            case AnimationFrameCopyOption::POSITION:
                m_target_sprite->set_position(m_position);
                break;
            case AnimationFrameCopyOption::SCALE:
                if(m_target_sprite->is_affine()) {
                    m_target_sprite->set_scale(m_scale);
                }
                break;
            case AnimationFrameCopyOption::ROTATION:
                if(m_target_sprite->is_affine()) {
                    m_target_sprite->set_rotation(static_cast<short>(m_rotation));
                }
                break;
            case AnimationFrameCopyOption::VISIBLE:
                if(m_visible) {
                    m_target_sprite->show();
                } else {
                    m_target_sprite->hide();
                }
                break;
            default:
                activate_on_target_sprite(copy_option);
                break;
        }
    }
}
