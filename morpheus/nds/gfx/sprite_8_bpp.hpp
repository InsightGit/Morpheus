//
// Created by bobby on 27/11/2020.
//

#ifndef MORPHEUS_NDS_SPRITE_HPP
#define MORPHEUS_NDS_SPRITE_HPP

#include <iostream>

#include <nds.h>

#include <core/core.hpp>
#include <nds/gfx/sprite.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite8Bpp : public nds::gfx::Sprite {
                public:
                    explicit Sprite8Bpp(bool use_sub_display, bool use_extended_palette = true) :
                        nds::gfx::Sprite(use_sub_display,SpriteMapping_1D_32,
                                        use_extended_palette ? ExtendedPaletteStatus::NEEDED :
                                                               ExtendedPaletteStatus::NEEDOFF) {}


                    // Single palette load functions
                    bool load_from_array(const unsigned short *tile_array, const unsigned int width,
                                         const unsigned int height);
                    bool load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                         const unsigned int width, const unsigned int height);

                    // Extended palette load functions
                    bool load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                         const unsigned int width, const unsigned int height)override;
                    bool load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                         const unsigned short *palette, const unsigned int width,
                                         const unsigned int height)override;
                    bool load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                           const unsigned int pal_len = 256)override;

                    bool load_from_pcx(const unsigned char *pcx_data, unsigned int palette_id = 0,
                                           bool copy_palette = true);
                protected:
                    void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority)override;
                private:
                    std::unique_ptr<sImage> m_sprite_image;
            };
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE_HPP*/
