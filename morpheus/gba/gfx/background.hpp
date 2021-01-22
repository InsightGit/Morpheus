//
// Created by bobby on 24/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_BACKGROUND_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <memory>

#include <tonc.h>

#include <core/gfx/vector_2.hpp>
#include <gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Background {
            public:
                explicit Background(unsigned int background_num, std::shared_ptr<GbaMainLoop> main_loop, bool is_8bpp,
                                    unsigned int cbb_num, unsigned int sbb_num);

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     const core::gfx::Vector2 size);
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     const core::gfx::Vector2 size);
            private:
                void update_background_register();

                int m_background_num;
                unsigned int m_background_register;
                unsigned int m_cbb_num;
                bool m_is_8bpp;
                std::shared_ptr<GbaMainLoop> m_main_loop;
                bool m_main_loop_notified = false;
                unsigned int m_sbb_num;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_BACKGROUND_HPP
