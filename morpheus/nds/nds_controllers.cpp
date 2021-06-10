//
// Created by bobby on 20/03/2021.
//

#include "nds_controllers.hpp"

void morpheus::nds::gfx::NdsMosaicController::update_mosaic_register() {
    core::gfx::Vector2 background_mosaic_levels = get_background_mosaic_levels();
    core::gfx::Vector2 sprite_mosaic_levels = get_sprite_mosaic_levels();

    unsigned int mosaic_register_value = background_mosaic_levels.get_x() | background_mosaic_levels.get_y() << 4 |
                                         sprite_mosaic_levels.get_x() << 8 | sprite_mosaic_levels.get_y() << 12;

    if(m_use_sub_display) {
        REG_MOSAIC_SUB = mosaic_register_value;
    } else {
        REG_MOSAIC = mosaic_register_value;
    }
}

void morpheus::nds::gfx::NdsBlendingController::update_blending_registers() {
    std::array<core::gfx::BlendingSetting, 4> background_blending_settings = get_background_blending_settings();
    bool update_alpha = false;
    bool update_fade = false;

    if(m_use_sub_display) {
        REG_BLDCNT_SUB = 0x0;
    } else {
        REG_BLDCNT = 0x0;
    }

    switch (get_blending_mode()) {
        case core::gfx::BlendingMode::OFF:
            // BM bits: 00
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x0000;
            } else {
                REG_BLDCNT |= 0x0000;
            }
            break;
        case core::gfx::BlendingMode::USE_WEIGHTS:
            // BM bits: 01
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x0040;
            } else {
                REG_BLDCNT |= 0x0040;
            }

            update_alpha = true;

            break;
        case core::gfx::BlendingMode::FADE_TO_WHITE:
            // BM bits: 10
            // 0100 0000
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x0080;
            } else {
                REG_BLDCNT |= 0x0080;
            }

            update_fade = true;

            break;
        case core::gfx::BlendingMode::FADE_TO_BLACK:
            // BM bits: 11
            // 0110 0000
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x00C0;
            } else {
                REG_BLDCNT |= 0x00C0;
            }

            update_fade = true;

            break;
    }

    switch(get_backdrop_blending_setting()) {
        case core::gfx::BlendingSetting::INVALID:
        case core::gfx::BlendingSetting::OFF:
            break;
        case core::gfx::BlendingSetting::BOTTOM_ON:
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= BD_BLENDING << BOTTOM_BLEND_SHIFT;
            } else {
                REG_BLDCNT |= BD_BLENDING << BOTTOM_BLEND_SHIFT;
            }
            break;
        case core::gfx::BlendingSetting::TOP_ON:
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= BD_BLENDING << TOP_BLEND_SHIFT;
            } else {
                REG_BLDCNT |= BD_BLENDING << TOP_BLEND_SHIFT;
            }
            break;
    }

    switch(get_object_blending_setting()) {
        case core::gfx::BlendingSetting::INVALID:
        case core::gfx::BlendingSetting::OFF:
            break;
        case core::gfx::BlendingSetting::BOTTOM_ON:
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= OBJ_BLENDING << BOTTOM_BLEND_SHIFT;
            } else {
                REG_BLDCNT |= OBJ_BLENDING << BOTTOM_BLEND_SHIFT;
            }
            break;
        case core::gfx::BlendingSetting::TOP_ON:
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= OBJ_BLENDING << TOP_BLEND_SHIFT;
            } else {
                REG_BLDCNT |= OBJ_BLENDING << TOP_BLEND_SHIFT;
            }
            break;
    }


    for(unsigned int i = 0; background_blending_settings.size() > i; ++i) {
        switch (background_blending_settings[i]) {
            case core::gfx::BlendingSetting::INVALID:
            case core::gfx::BlendingSetting::OFF:
                break;
            case core::gfx::BlendingSetting::BOTTOM_ON:
                if(m_use_sub_display) {
                    REG_BLDCNT_SUB |= get_blending_value(i) << BOTTOM_BLEND_SHIFT;
                } else {
                    REG_BLDCNT |= get_blending_value(i) << BOTTOM_BLEND_SHIFT;
                }

                break;
            case core::gfx::BlendingSetting::TOP_ON:
                if(m_use_sub_display) {
                    REG_BLDCNT_SUB |= get_blending_value(i) << TOP_BLEND_SHIFT;
                } else {
                    REG_BLDCNT |= get_blending_value(i) << TOP_BLEND_SHIFT;
                }

                break;
        }
    }

    if(update_alpha) {
        if(m_use_sub_display) {
            REG_BLDALPHA_SUB = (get_blend_weight(false) << EVA_BLEND_SHIFT) |
                                (get_blend_weight(true) << EVB_BLEND_SHIFT);
        } else {
            REG_BLDALPHA = (get_blend_weight(false) << EVA_BLEND_SHIFT) |
                            (get_blend_weight(true) << EVB_BLEND_SHIFT);
        }
    } else if(update_fade) {
        if(m_use_sub_display) {
            REG_BLDY_SUB = get_blend_fade() << Y_BLEND_SHIFT;
        } else {
            REG_BLDY = get_blend_fade() << Y_BLEND_SHIFT;
        }
    }
}
