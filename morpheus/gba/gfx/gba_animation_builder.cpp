//
// Created by bobby on 25/04/2021.
//

#include <gba/gfx/sprite.hpp>

#include "gba_animation_builder.hpp"

morpheus::gba::gfx::GbaAnimationBuilder::GbaAnimationBuilder(gba::gfx::Sprite *from) :
    core::gfx::AnimationBuilder(from) {
}

void morpheus::gba::gfx::GbaAnimationBuilder::add_frame(GbaAnimationFrame frame,
                                                        std::vector<GbaAnimationFrameCopyOption> copy_options) {
    core::gfx::AnimationFrame general_new_frame_to_add = m_animation_frames[m_animation_frames.size() - 1];
    GbaAnimationFrame gba_animation_frame = static_cast<GbaAnimationFrame>(general_new_frame_to_add);

    for(auto &copy_option : copy_options) {
        switch(copy_option) {
            case GbaAnimationFrameCopyOption::TILE_ID:
                new_frame_to_add.tile_id = frame.tile_id;
                break;
            default:
                copy_appropriate_frame_attribute(new_frame_to_add, frame,
                                                 static_cast<core::gfx::AnimationFrameCopyOption>(copy_option));
                break;
        }
    }

    m_animation_frames.push_back(new_frame_to_add);
}


