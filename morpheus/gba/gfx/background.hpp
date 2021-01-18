//
// Created by bobby on 24/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_BACKGROUND_HPP

#include <algorithm>
#include <cstring>
#include <memory>

#include <tonc.h>

#include <gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Background {
            public:
                explicit Background(unsigned int background_num, std::shared_ptr<GbaMainLoop> main_loop);

                void load_from_array(const unsigned int *tiles, const int tiles_len,
                                     const unsigned short *palette, const int pal_len, const unsigned short *tile_map,
                                     const int map_len);
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map);
            private:
                int m_background_num;
                unsigned short m_bg_control_register;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_BACKGROUND_HPP
