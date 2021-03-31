//
// Created by bobby on 20/03/2021.
//

#include "nds_controllers.hpp"

void morpheus::nds::gfx::NdsBlendingController::disable_backdrop_blending() {
    // Top (shift 0) then bottom (shift 8)
    if(m_use_sub_display) {
        REG_BLDCNT_SUB &= ~(BD_BLENDING << 0 | BD_BLENDING << 8);
    } else {
        REG_BLDCNT &= ~(BD_BLENDING << 0 | BD_BLENDING << 8);
    }
}

void morpheus::nds::gfx::NdsBlendingController::enable_backdrop_blending(bool bottom) {
    // gets rid of accidentally setting bottom and top bits on backdrop
    disable_backdrop_blending();

    if(bottom) {
        if(m_use_sub_display) {
            REG_BLDCNT_SUB |= BD_BLENDING << BOTTOM_BLEND_SHIFT;
        } else {
            REG_BLDCNT |= BD_BLENDING << BOTTOM_BLEND_SHIFT;
        }
    } else {
        if(m_use_sub_display) {
            REG_BLDCNT_SUB |= BD_BLENDING << TOP_BLEND_SHIFT;
        } else {
            REG_BLDCNT |= BD_BLENDING << TOP_BLEND_SHIFT;
        }
    }
}

void morpheus::nds::gfx::NdsBlendingController::disable_background_blending(unsigned int background_num) {
    unsigned char blending_value;

    background_num = std::min(3u, background_num);
    blending_value = get_blending_value(background_num);

    if(m_use_sub_display) {
        REG_BLDCNT_SUB &= ~(blending_value << TOP_BLEND_SHIFT | blending_value << BOTTOM_BLEND_SHIFT);
    } else {
        REG_BLDCNT &= ~(blending_value << TOP_BLEND_SHIFT | blending_value << BOTTOM_BLEND_SHIFT);
    }
}

void morpheus::nds::gfx::NdsBlendingController::enable_background_blending(bool bottom, unsigned int background_num) {
    unsigned char blending_value;

    background_num = std::min(3u, background_num);
    blending_value = get_blending_value(background_num);

    // gets rid of accidentally setting bottom and top bits on background
    disable_background_blending(background_num);

    if(bottom) {
        if(m_use_sub_display) {
            REG_BLDCNT_SUB |= blending_value << BOTTOM_BLEND_SHIFT;
        } else {
            REG_BLDCNT |= blending_value << BOTTOM_BLEND_SHIFT;
        }
    } else {
        if(m_use_sub_display) {
            REG_BLDCNT_SUB |= blending_value << TOP_BLEND_SHIFT;
        } else {
            REG_BLDCNT |= blending_value << TOP_BLEND_SHIFT;
        }
    }
}

void morpheus::nds::gfx::NdsBlendingController::disable_object_blending() {
    if(m_use_sub_display) {
        REG_BLDCNT_SUB &= ~(OBJ_BLENDING << TOP_BLEND_SHIFT | OBJ_BLENDING << BOTTOM_BLEND_SHIFT);
    } else {
        REG_BLDCNT &= ~(OBJ_BLENDING << TOP_BLEND_SHIFT | OBJ_BLENDING << BOTTOM_BLEND_SHIFT);
    }
}

void morpheus::nds::gfx::NdsBlendingController::enable_object_blending(bool bottom) {
    // gets rid of accidentally setting bottom and top bits on backdrop
    disable_object_blending();

    if(bottom) {
        if(m_use_sub_display) {
            REG_BLDCNT_SUB |= OBJ_BLENDING << BOTTOM_BLEND_SHIFT;
        } else {
            REG_BLDCNT |= OBJ_BLENDING << BOTTOM_BLEND_SHIFT;
        }
    } else {
        if(m_use_sub_display) {
            REG_BLDCNT_SUB |= OBJ_BLENDING << TOP_BLEND_SHIFT;
        } else {
            REG_BLDCNT |= OBJ_BLENDING << TOP_BLEND_SHIFT;
        }
    }
}

unsigned char morpheus::nds::gfx::NdsBlendingController::get_blend_weight(bool bottom) const {
    if(bottom) {
        if(m_use_sub_display) {
            return REG_BLDALPHA_SUB & EVB_BLEND_MASK;
        } else {
            return REG_BLDALPHA & EVB_BLEND_MASK;
        }
    } else {
        if(m_use_sub_display) {
            return REG_BLDALPHA_SUB & EVA_BLEND_MASK;
        } else {
            return REG_BLDALPHA & EVA_BLEND_MASK;
        }
    }
}

void morpheus::nds::gfx::NdsBlendingController::set_blend_weight(bool bottom, unsigned char weight) {
    if(bottom) {
        if(m_use_sub_display) {
            REG_BLDALPHA_SUB |= weight << EVB_BLEND_SHIFT;
        } else {
            REG_BLDALPHA |= weight << EVB_BLEND_SHIFT;
        }
    } else {
        if(m_use_sub_display) {
            REG_BLDALPHA_SUB |= weight << EVA_BLEND_SHIFT;
        } else {
            REG_BLDALPHA |= weight << EVA_BLEND_SHIFT;
        }
    }
}

unsigned char morpheus::nds::gfx::NdsBlendingController::get_blend_fade() const {
    if(m_use_sub_display) {
        return REG_BLDY_SUB & Y_BLEND_MASK;
    } else {
        return REG_BLDY & Y_BLEND_MASK;
    }
}

void morpheus::nds::gfx::NdsBlendingController::set_blend_fade(unsigned char fade) {
    if(m_use_sub_display) {
        REG_BLDY_SUB = fade << Y_BLEND_SHIFT;
    } else {
        REG_BLDY = fade << Y_BLEND_SHIFT;
    }
}

morpheus::core::gfx::BlendingMode morpheus::nds::gfx::NdsBlendingController::get_blending_mode() {
    unsigned int blending_mode;

    if(m_use_sub_display) {
        blending_mode = (REG_BLDCNT_SUB >> 6) & 0x0003;
    } else {
        blending_mode = (REG_BLDCNT >> 6) & 0x0003;
    }

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

void morpheus::nds::gfx::NdsBlendingController::set_blending_mode(morpheus::core::gfx::BlendingMode blending_mode) {
    // Clears current blending mode
    if(m_use_sub_display) {
        REG_BLDCNT_SUB &= 0xFF3F;
    } else {
        REG_BLDCNT &= 0xFF3F;
    }

    switch (blending_mode) {
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
            nocashMessage("Setting to use weights");
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x0040;
                nocashMessage("on sub");
            } else {
                REG_BLDCNT |= 0x0040;
                nocashMessage("on main");
            }
            break;
        case core::gfx::BlendingMode::FADE_TO_WHITE:
            // BM bits: 10
            // 0100 0000
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x0080;
            } else {
                REG_BLDCNT |= 0x0080;
            }
            break;
        case core::gfx::BlendingMode::FADE_TO_BLACK:
            // BM bits: 11
            // 0110 0000
            if(m_use_sub_display) {
                REG_BLDCNT_SUB |= 0x00C0;
            } else {
                REG_BLDCNT |= 0x00C0;
            }
            break;
    }
}

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
