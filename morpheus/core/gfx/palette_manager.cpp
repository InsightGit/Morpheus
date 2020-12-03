//
// Created by bobby on 28/11/2020.
//

/*#include "palette_manager.hpp"

bool morpheus::core::gfx::PaletteManager::load_colors_to_palette(const std::vector<unsigned short> colors,
                                                                 int &translation_map_id) {
    // TODO(Bobby): Make this work for palettes that aren't null-terminated
    for(int i = 0; PALBANK_SIZE > i; ++i) {
        if(colors.empty()) {
            break;
        }

        if(m_palette_bank[i] == 0x0) {
            m_palette_bank[i] = colors[0];

            colors.erase(colors.begin());
        } else  {
            for(int i2 = 0; colors.size() > i2; ++i2) {
                if(m_palbank_address[i] == colors[i2]) {
                    //colors.erase(colors.begin() + i2);

                    break;
                }
            }
        }
    }

    //translation_map_id

    update_palbank();

    return colors.empty();
    return true;
}

void morpheus::core::gfx::PaletteManager::update_palbank() const {
    // TODO(Bobby): Investigate optimizing this through deltas
    //std::memcopy(m_palbank_address, m_palette, PALBANK_SIZE);
}*/
