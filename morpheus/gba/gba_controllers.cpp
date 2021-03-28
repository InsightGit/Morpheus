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
    weight = std::min(static_cast<unsigned int>(weight), 32u);

    if(bottom) {
        REG_BLDALPHA &= ~BLD_EVB_MASK;

        REG_BLDALPHA |= BLD_EVB(weight);
    } else {
        REG_BLDALPHA &= ~BLD_EVA_MASK;

        REG_BLDALPHA |= BLD_EVA(weight);
    }

    /*nocash_puts("Blending weight set to");
    nocash_puts(std::string(std::to_string(static_cast<unsigned int>(weight))).c_str());*/
}

unsigned char morpheus::gba::gfx::GbaBlendingController::get_blend_fade() const {
    return REG_BLDY & BLDY_MASK;
}

void morpheus::gba::gfx::GbaBlendingController::set_blend_fade(unsigned char fade) {
    REG_BLDY = 0x0;

    REG_BLDY |= (fade) << BLDY_SHIFT;
}

morpheus::core::gfx::BlendingMode morpheus::gba::gfx::GbaBlendingController::get_blending_mode() {
    unsigned int blending_mode = (REG_BLDCNT >> 6) & 0x0003;

    switch(blending_mode) {
        case 0:
            return morpheus::core::gfx::BlendingMode::OFF;
        case 1:
            return morpheus::core::gfx::BlendingMode::USE_WEIGHTS;
        case 2:
            return morpheus::core::gfx::BlendingMode::FADE_TO_WHITE;
        case 3:
            return morpheus::core::gfx::BlendingMode::FADE_TO_BLACK;
        default:
            return morpheus::core::gfx::BlendingMode::OFF;
    }
}

void morpheus::gba::gfx::GbaBlendingController::set_blending_mode(morpheus::core::gfx::BlendingMode blending_mode) {
    // Clears current blending mode
    REG_BLDCNT &= 0xFF3F;

    switch (blending_mode) {
        case core::gfx::BlendingMode::OFF:
            // BM bits: 00
            REG_BLDCNT |= BLD_OFF;
            break;
        case core::gfx::BlendingMode::USE_WEIGHTS:
            // BM bits: 01
            REG_BLDCNT |= BLD_STD;
            break;
        case core::gfx::BlendingMode::FADE_TO_WHITE:
            // BM bits: 10
            REG_BLDCNT |= BLD_WHITE;
            break;
        case core::gfx::BlendingMode::FADE_TO_BLACK:
            // BM bits: 11
            REG_BLDCNT |= BLD_BLACK;
            break;
    }
}

void morpheus::gba::gfx::GbaMosaicController::update_mosaic_register() {
    core::gfx::Vector2 background_mosaic_levels = get_background_mosaic_levels();
    core::gfx::Vector2 sprite_mosaic_levels = get_sprite_mosaic_levels();

    REG_MOSAIC = MOS_BUILD(background_mosaic_levels.get_x(), background_mosaic_levels.get_y(),
                           sprite_mosaic_levels.get_x(), sprite_mosaic_levels.get_y());
}
