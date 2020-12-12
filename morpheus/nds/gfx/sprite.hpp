//
// Created by bobby on 27/11/2020.
//

#ifndef MORPHEUS_NDS_SPRITE_HPP
#define MORPHEUS_NDS_SPRITE_HPP

#include <stdio.h>

#include <core/core.hpp>

#include <nds.h>

namespace morpheus {
    namespace nds {
        namespace gfx {
            // TODO(Bobby): Make 16-color GBA-style sprite type for nds morpheus
            class Sprite : public core::Node {
                public:
                    Sprite(bool use_sub_display);

                    core::gfx::Vector2 get_position() const {
                        return m_position;
                    }

                    void set_position(const core::gfx::Vector2 position) {
                        m_position = position;
                    }

                    void set_position(const int x, const int y) {
                        set_position(core::gfx::Vector2(x, y));
                    }

                    void load_from_array(uint8_t **tile_array, uint8_t tile_array_len,
                                         uint8_t **pal, uint8_t pal_len, uint8_t width,
                                         uint8_t height, uint8_t tile_id);
                    bool load_from_pcx(const unsigned char *pcx_data, uint8_t palette_id, bool copy_palette = true);

                    void draw(std::vector<void *>obj_attr_buffer, int obj_attr_num)override;
                protected:
                    void draw_children(std::vector<void *>obj_attr_buffer, int obj_attr_num)override {};

                    void set_sprite_size(uint8_t width, uint8_t height);

                    virtual void input(core::InputEvent input_event)override {}
                private:
                    core::gfx::Vector2 m_position;

                    uint16_t *m_gfx_pointer;
                    uint8_t m_palette_id;

                    OamState *m_current_oam;
                    std::unique_ptr<sImage> m_sprite_image;
                    SpriteSize m_sprite_size;
            };
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE_HPP*/
