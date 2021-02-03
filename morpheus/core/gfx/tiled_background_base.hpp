//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP

#include <algorithm>

#include <core/main_loop.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class TiledBackgroundSize {
                BG_32x32,
                BG_64x32,
                BG_32x64,
                BG_64x64
            };

            class TiledBackgroundBase {
            public:
                TiledBackgroundBase(unsigned int background_num, unsigned int cbb_num, unsigned int sbb_num);

                virtual void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                             const unsigned short *palette, const unsigned int pal_len,
                                             const unsigned short *tile_map, const unsigned int tile_map_len,
                                             TiledBackgroundSize size) = 0;
                virtual void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                             const unsigned short *tile_map, const unsigned int tile_map_len,
                                             TiledBackgroundSize size) = 0;

                Vector2 get_scroll() const {
                    return m_scroll_position;
                }

                void set_scroll(Vector2 scroll_position) {
                    m_scroll_position = scroll_position;

                    update_scroll();
                }

                virtual unsigned int get_priority() const = 0;
                virtual void set_priority(unsigned int priority) = 0;
            protected:
                unsigned int get_background_num() const {
                    return m_background_num;
                }

                unsigned int get_cbb_num() const {
                    return m_cbb_num;
                }

                unsigned int get_sbb_num() const {
                    return m_sbb_num;
                }

                virtual void update_scroll() = 0;
            private:
                unsigned int m_background_num;
                unsigned int m_cbb_num;
                unsigned int m_sbb_num;
                Vector2 m_scroll_position;
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
