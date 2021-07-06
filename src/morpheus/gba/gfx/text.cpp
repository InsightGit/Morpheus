//
// Created by bobby on 09/04/2021.
//

#include "morpheus/gba/gfx/text.hpp"

void morpheus::gba::gfx::Text::print_chars(std::string string, bool init) {
    if(init) {
        if(is_affine()) {
            tte_init_ase(static_cast<int>(get_background_num()), BG_CBB(get_cbb()) | BG_SBB(get_sbb()), 0,
                         CLR_WHITE, 14, nullptr, nullptr);
        } else {
            tte_init_se(static_cast<int>(get_background_num()), BG_CBB(get_cbb()) | BG_SBB(get_sbb()), 0,
                        CLR_WHITE,14, nullptr, nullptr);
        }

        // update print position
        change_print_position(get_print_position());
    }

    tte_write(string.c_str());
}
