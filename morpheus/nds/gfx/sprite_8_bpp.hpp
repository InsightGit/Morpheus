//
// Created by bobby on 27/11/2020.
//

#ifndef MORPHEUS_NDS_SPRITE8BPP_HPP
#define MORPHEUS_NDS_SPRITE8BPP_HPP

#include <iostream>

#include <nds.h>

#include <core/core.hpp>
#include <nds/gfx/sprite.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite8Bpp : public nds::gfx::Sprite {
                public:
                    explicit Sprite8Bpp(bool use_sub_display, NdsBlendingController *blending_controller,
                                        NdsMosaicController *mosaic_controller, bool use_extended_palette = true) :
                        nds::gfx::Sprite(use_sub_display, blending_controller, mosaic_controller, SpriteMapping_1D_32,
                                        use_extended_palette ? ExtendedPaletteStatus::NEEDED :
                                                               ExtendedPaletteStatus::NEEDOFF) {}

                    ~Sprite8Bpp() override = default;

                    // Single palette load functions
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const morpheus::core::gfx::SpriteSize size);
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette, const unsigned int palette_len,
                                         const morpheus::core::gfx::SpriteSize size);
                    bool load_into_palette(const unsigned short *palette, const unsigned int palette_len) override {
                        return load_into_palette(palette, palette_len, 0);
                    }

                    // Extended palette load functions
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned int palette_id,
                                         const morpheus::core::gfx::SpriteSize size)override;
                    bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette,
                                         const unsigned int palette_len, const unsigned int palette_id,
                                         const morpheus::core::gfx::SpriteSize size)override;
                    bool load_into_palette(const unsigned short *palette, const unsigned int palette_len,
                                           const unsigned int palette_id);

                    bool load_from_pcx(const unsigned char *pcx_data, unsigned int palette_id = 0,
                                       bool copy_palette = true);
                protected:
                    void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num)override;
                private:
                    std::unique_ptr<sImage> m_sprite_image;
            };
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE8BPP_HPP*/
