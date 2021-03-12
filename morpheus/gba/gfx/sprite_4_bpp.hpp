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
                explicit Sprite4Bpp(const unsigned int tile_id, const unsigned int palette_id,
                                    const unsigned int width, const unsigned int height);

                virtual ~Sprite4Bpp() = default;

                unsigned int get_palette_id() const {
                    return m_palette_id;
                }

                void set_palette_id(const unsigned int palette_id) {
                    m_palette_id = std::min(palette_id, 15u);

                    build_attr2(m_palette_id, m_tile_id);
                }

                void load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                     const unsigned int palette_len, const unsigned int palette_id,
                                     const unsigned int width, const unsigned int height,
                                     const unsigned int tile_id);
                void load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                     const unsigned int width, const unsigned int height,
                                     const unsigned int tile_id);

                void load_into_palette(const unsigned short *palette, const unsigned int palette_len)override;
            protected:
                void array_load(const unsigned short *tile_array, const unsigned int width,
                                const unsigned int height, const unsigned int tile_id)override;
                void array_load(const unsigned short *tile_array, const unsigned short *palette,
                                const unsigned int palette_len, const unsigned int width,
                                const unsigned int height, const unsigned int tile_id)override;
            private:
                unsigned int m_palette_id;
                unsigned int m_tile_id;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_4_BPP_HPP
