//
// Created by bobby on 20/03/2021.
//

#include "morpheus/gba/gba_controllers.hpp"

void morpheus::gba::gfx::GbaBlendingController::update_blending_registers() {
    std::array<core::gfx::BlendingSetting, 4> background_blending_settings = get_background_blending_settings();
    bool update_alpha = false;
    bool update_fade = false;

    REG_BLDCNT = 0x0;

    switch (get_blending_mode()) {
        case core::gfx::BlendingMode::OFF: {
            // BM bits: 00
            REG_BLDCNT |= BLD_OFF;

            break;
        }
        case core::gfx::BlendingMode::USE_WEIGHTS: {
            // BM bits: 01
            REG_BLDCNT |= BLD_STD;
            update_alpha = true;

            break;
        }
        case core::gfx::BlendingMode::FADE_TO_WHITE: {
            // BM bits: 10
            REG_BLDCNT |= BLD_WHITE;
            update_fade = true;

            break;
        }
        case core::gfx::BlendingMode::FADE_TO_BLACK: {
            // BM bits: 11
            REG_BLDCNT |= BLD_BLACK;
            update_fade = true;

            break;
        }
    }

    switch(get_backdrop_blending_setting()) {
        case core::gfx::BlendingSetting::INVALID:
        case core::gfx::BlendingSetting::OFF:
            break;
        case core::gfx::BlendingSetting::BOTTOM_ON:
            REG_BLDCNT |= BLD_BOT(BD_BLENDING);
            break;
        case core::gfx::BlendingSetting::TOP_ON:
            REG_BLDCNT |= BLD_TOP(BD_BLENDING);
            break;
    }

    for(unsigned int i = 0; background_blending_settings.size() > i; ++i) {
        switch (background_blending_settings[i]) {
            case core::gfx::BlendingSetting::INVALID:
            case core::gfx::BlendingSetting::OFF:
                break;
            case core::gfx::BlendingSetting::BOTTOM_ON:
                REG_BLDCNT |= BLD_BOT(get_blending_value(i));
                break;
            case core::gfx::BlendingSetting::TOP_ON:
                REG_BLDCNT |= BLD_TOP(get_blending_value(i));
                break;
        }
    }

    switch (get_object_blending_setting()) {
        case core::gfx::BlendingSetting::INVALID:
        case core::gfx::BlendingSetting::OFF:
            break;
        case core::gfx::BlendingSetting::BOTTOM_ON:
            REG_BLDCNT |= BLD_BOT(OBJ_BLENDING);
            break;
        case core::gfx::BlendingSetting::TOP_ON:
            REG_BLDCNT |= BLD_TOP(OBJ_BLENDING);
            break;
    }

    if(update_alpha) {
        REG_BLDALPHA = BLD_EVA(get_blend_weight(false)) | BLD_EVB(get_blend_weight(true));
    } else if(update_fade) {
        REG_BLDY = get_blend_fade() << BLDY_SHIFT;
    }
}

void morpheus::gba::gfx::GbaMosaicController::update_mosaic_register() {
    core::gfx::Vector2 background_mosaic_levels = get_background_mosaic_levels();
    core::gfx::Vector2 sprite_mosaic_levels = get_sprite_mosaic_levels();

    REG_MOSAIC = MOS_BUILD(background_mosaic_levels.get_x(), background_mosaic_levels.get_y(),
                           sprite_mosaic_levels.get_x(), sprite_mosaic_levels.get_y());
}
