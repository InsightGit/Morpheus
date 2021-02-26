//
// Created by bobby on 22/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
#define MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP

#include <gba/gfx/sprite.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite4Bpp : public morpheus::gba::gfx::Sprite {
            public:
                explicit Sprite4Bpp(const unsigned int palette_id = 0);
                explicit Sprite4Bpp(const unsigned short tile_id, const unsigned int palette_id,
                                    const unsigned short width, const unsigned short height);

                int get_palette_id() const {
                    return m_palette_id;
                }

                void set_palette_id(const unsigned int palette_id) {
                    m_palette_id = std::min(palette_id, 15u);
                }

                void load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                     const unsigned short palette_id, const unsigned short width,
                                     const unsigned short height, const unsigned short tile_id);
                void load_from_array(const unsigned short *tile_array, const unsigned short palette_id,
                                     const unsigned short width, const unsigned short height,
                                     const unsigned short tile_id);

                void load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                       const unsigned int pal_len = 32)override;
            protected:
                void array_load(const unsigned short *tile_array, const unsigned short width,
                                const unsigned short height, const unsigned short tile_id)override;
                void array_load(const unsigned short *tile_array, const unsigned short *palette,
                                const unsigned short width, const unsigned short height,
                                const unsigned short tile_id)override;
            private:
                unsigned short m_palette_id;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
