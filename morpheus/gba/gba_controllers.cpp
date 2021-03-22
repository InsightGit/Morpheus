//
// Created by bobby on 20/03/2021.
//

#include "gba_controllers.hpp"

void morpheus::gba::gfx::GbaBlendingController::disable_backdrop_blending() {
    REG_BLDCNT &= ~(BLD_TOP(BD_BLENDING) | BLD_BOT(BD_BLENDING));
}

void morpheus::gba::gfx::GbaBlendingController::enable_backdrop_blending(bool bottom) {
    // gets rid of accidentally setting bottom and top bits on backdrop
    disable_backdrop_blending();

    if(bottom) {
        REG_BLDCNT |= BLD_BOT(BD_BLENDING);
    } else {
        REG_BLDCNT |= BLD_TOP(BD_BLENDING);
    }
}

void morpheus::gba::gfx::GbaBlendingController::disable_background_blending(unsigned int background_num) {
    unsigned char blending_value;

    background_num = std::min(3u, background_num);
    blending_value = get_blending_value(background_num);

    REG_BLDCNT &= ~(BLD_TOP(blending_value) | BLD_BOT(blending_value));
}

void morpheus::gba::gfx::GbaBlendingController::enable_background_blending(bool bottom, unsigned int background_num) {
    unsigned char blending_value;

    background_num = std::min(3u, background_num);
    blending_value = get_blending_value(background_num);

    // gets rid of accidentally setting bottom and top bits on background
    disable_background_blending(background_num);

    if(bottom) {
        REG_BLDCNT |= BLD_BOT(blending_value);
    } else {
        REG_BLDCNT |= BLD_TOP(blending_value);
    }
}

void morpheus::gba::gfx::GbaBlendingController::disable_object_blending() {
    REG_BLDCNT &= ~(BLD_TOP(OBJ_BLENDING) | BLD_BOT(OBJ_BLENDING));
}

void morpheus::gba::gfx::GbaBlendingController::enable_object_blending(bool bottom) {
    // gets rid of accidentally setting bottom and top bits on backdrop
    disable_object_blending();

    if(bottom) {
        REG_BLDCNT |= BLD_BOT(OBJ_BLENDING);
    } else {
        REG_BLDCNT |= BLD_TOP(OBJ_BLENDING);
    }
}

unsigned char morpheus::gba::gfx::GbaBlendingController::get_blend_weight(bool bottom) const {
    if(bottom) {
        return REG_BLDALPHA & BLD_EVB_MASK;
    } else {
        return REG_BLDALPHA & BLD_EVA_MASK;
    }
}

void morpheus::gba::gfx::GbaBlendingController::set_blend_weight(bool bottom, unsigned char weight) {
    if(bottom) {
        REG_BLDALPHA |= BLD_EVA(weight);
    } else {
        REG_BLDALPHA |= BLD_EVB(weight);
    }
}

unsigned char morpheus::gba::gfx::GbaBlendingController::get_blend_fade() const {
    return REG_BLDY & BLDY_MASK;
}

void morpheus::gba::gfx::GbaBlendingController::set_blend_fade(unsigned char fade) {
    REG_BLDY |= (fade) << BLDY_SHIFT;
}
