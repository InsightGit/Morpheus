//
// Created by bobby on 24/04/2021.
//

#include "animation_builder.hpp"

morpheus::core::gfx::AnimationBuilder::AnimationBuilder(morpheus::core::gfx::SpriteBase *from) {
    AnimationFrame source;

    source.blending_value = from->get_blending_controller()->get_blend_weight(
            from->get_blending_controller()->get_object_blending_setting() == morpheus::core::gfx::BlendingSetting::BOTTOM_ON);
    source.mosaic_levels = from->get_mosaic_levels();
    source.rotation = from->get_rotation();
    source.scale = from->get_scale();
    source.sprite_size = from->get_sprite_size();
    source.visible = !from->is_hidden();
    source.vblank_delays = 0;

    m_animation_frames.push_back(source);
}

void morpheus::core::gfx::AnimationBuilder::copy_appropriate_frame_attribute(
                                                              morpheus::core::gfx::AnimationFrame &dest,
                                                              morpheus::core::gfx::AnimationFrame source,
                                                              AnimationFrameCopyOption copy_option) {
    switch(copy_option) {
        case core::gfx::AnimationFrameCopyOption::BLENDING:
            dest.blending_value = source.blending_value;
            break;
        case core::gfx::AnimationFrameCopyOption::SPRITE_SIZE:
            dest.enable_size_animation = source.enable_size_animation;
            dest.sprite_size = source.sprite_size;
            break;
        case core::gfx::AnimationFrameCopyOption::MOSAIC:
            dest.mosaic_levels = source.mosaic_levels;
            break;
        case core::gfx::AnimationFrameCopyOption::PALETTE:
            dest.palette_id = source.palette_id;
            break;
        case core::gfx::AnimationFrameCopyOption::SCALE:
            dest.scale = source.scale;
            break;
        case core::gfx::AnimationFrameCopyOption::ROTATION:
            dest.rotation = source.rotation;
            break;
        case core::gfx::AnimationFrameCopyOption::VISIBLE:
            dest.visible = source.visible;
            break;
        default:
            break;

    }
}
