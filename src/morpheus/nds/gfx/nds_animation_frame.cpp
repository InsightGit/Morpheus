//
// Created by bobby on 01/05/2021.
//

#include <morpheus/nds/gfx/sprite.hpp>

#include "morpheus/nds/gfx/nds_animation_frame.hpp"

morpheus::nds::gfx::NdsAnimationFrame::NdsAnimationFrame(morpheus::nds::gfx::Sprite *target_sprite,
                                                         morpheus::nds::gfx::NdsAnimationFrame *from_animation_frame) :
        core::gfx::AnimationFrame(target_sprite, from_animation_frame) {
    if(from_animation_frame != nullptr) {
        m_gfx_pointer = from_animation_frame->m_gfx_pointer;
    }
}

void morpheus::nds::gfx::NdsAnimationFrame::activate_on_target_sprite(
        morpheus::core::gfx::AnimationFrameCopyOption copy_option) {
    auto *nds_sprite = static_cast<nds::gfx::Sprite*>(get_target_sprite());

    switch(static_cast<NdsAnimationFrameCopyOption>(copy_option)) {
        case NdsAnimationFrameCopyOption::GFX_PTR:
            nds_sprite->disable_gfx_pointer_frees();

            nds_sprite->set_gfx_pointer(m_gfx_pointer);

            nds_sprite->enable_gfx_pointer_frees();
            break;
    }
}

morpheus::nds::gfx::NdsAnimationFrame::~NdsAnimationFrame() {
    if(m_gfx_pointer != nullptr) {
        // TODO(Bobby): Properly free gfx pointer here

        /*static_cast<morpheus::nds::gfx::Sprite*>(get_target_sprite())->disable_gfx_pointer_frees();
        static_cast<morpheus::nds::gfx::Sprite*>(get_target_sprite())->disable_gfx_pointer_frees();

        static_cast<morpheus::nds::gfx::Sprite*>(get_target_sprite())->set_gfx_pointer(nullptr);*/
    }
}
