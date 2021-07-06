//
// Created by bobby on 22/03/2021.
//

#include "morpheus/core/controllers.hpp"

unsigned char morpheus::core::gfx::BlendingController::get_blending_value(unsigned int background_num) {
    switch(background_num) {
        case 0u:
            return BG_0_BLENDING;
        case 1u:
            return BG_1_BLENDING;
        case 2u:
            return BG_2_BLENDING;
        case 3u:
            return BG_3_BLENDING;
        default:
            return 0;
    }
}

