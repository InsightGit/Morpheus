//
// Created by bobby on 28/11/2020.
//

/*#ifndef MORPHEUS_CORE_PALETTE_MANAGER_HPP
#define MORPHEUS_CORE_PALETTE_MANAGER_HPP

#include <map>
#include <memory>
#include <vector>

namespace morpheus {
    namespace core {
        namespace gfx {
            class PaletteManager {
                public:
                    PaletteManager(const uint16_t palbank_address) {
                        m_palbank_address = palbank_address;
                    }

                    bool load_colors_to_palette(const std::vector<unsigned short> colors, int &translation_map_id);

                    bool load_colors_to_palette(const unsigned short (*colors)[]) {
                        return load_colors_to_palette(std::vector<unsigned short>(*colors));
                    }
                private:
                    static const int PALBANK_SIZE = 256;

                    void update_palbank()const;

                    // TODO(Bobby): use a more performant data structure than std::map
                    std::vector<std::map<unsigned int, unsigned short>> m_translation_tables;

                    uint16_t m_palbank_address;
                    unsigned short m_palette[PALBANK_SIZE];
            };
        }
    }
}

#endif //MORPHEUS_CORE_PALETTE_MANAGER_HPP*/
