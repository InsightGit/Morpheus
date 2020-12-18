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
            // TODO(Bobby): Make 16-color GBA-style sprite type for nds morpheus
            class Sprite8Bpp : public nds::gfx::Sprite {
                public:
                    explicit Sprite8Bpp(bool use_sub_display) : nds::gfx::Sprite(use_sub_display,SpriteMapping_1D_32, true) {}


                    // Single palette load functions
                    bool load_from_array(uint8_t **tile_array, uint8_t width, uint8_t height)override;
                    bool load_from_array(uint8_t **tile_array, uint16_t **palette, uint8_t width,
                                         uint8_t height)override;

                    // Extended palette load functions
                    bool load_from_array(uint8_t **tile_array, uint8_t palette_id, uint8_t width,
                                         uint8_t height)override;
                    bool load_from_array(uint8_t **tile_array, uint8_t palette_id,
                                         uint16_t **palette, uint8_t width, uint8_t height)override;

                    bool load_from_pcx(const unsigned char *pcx_data, uint8_t palette_id = 0, bool copy_palette = true);

                    void draw(std::vector<void *>obj_attr_buffer, int obj_attr_num)override;
                protected:
                    void draw_children(std::vector<void *>obj_attr_buffer, int obj_attr_num)override {};
                private:
                    uint8_t m_palette_id;

                    std::unique_ptr<sImage> m_sprite_image;

                    void copy_into_palette(uint16_t **palette, uint8_t palette_id);
            };
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE_HPP*/
