//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_SPRITE_HPP
#define MORPHEUS_SPRITE_HPP

#include <cassert>
#include <cstring>

#include <tonc.h>
#include <tonc_video.h>

#include <core/node.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite : public core::Node {
                public:
                    Sprite() = default;

                    void load_from_array(unsigned short **tile_array, unsigned short tile_array_len,
                                         unsigned short **pal, unsigned short pal_len,
                                         unsigned short bit_depth, unsigned short width,
                                         unsigned short height, unsigned short tile_id);

                    void **draw(void *obj_attr_buffer[], int obj_attr_num = 0)override;

                    void draw_children(void *obj_attr_buffer[], int obj_attr_num)override;

                    core::gfx::Vector2 get_position() const {
                        return m_position;
                    }

                    void set_position(const core::gfx::Vector2 position) {
                        m_position = position;
                    }

                    void set_position(const int x, const int y) {
                        set_position(core::gfx::Vector2(x, y));
                    }
                protected:
                    void input()override {}
                private:
                    unsigned int m_attr0;
                    unsigned int m_attr1;
                    unsigned int m_attr2;

                    unsigned short m_bit_depth;
                    unsigned short m_tile_id;
                    unsigned short **m_tile_array;
                    unsigned short m_tile_array_len;
                    unsigned short **m_pal;
                    unsigned short m_pal_len;

                    core::gfx::Vector2 m_position;
            };
        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
