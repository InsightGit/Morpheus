//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_SPRITE_HPP
#define MORPHEUS_SPRITE_HPP

#include <cassert>
#include <cstring>

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

                    void draw(OBJ_ATTR (*obj_attr_buffer)[], int obj_attr_num = 0)override;

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
                    void draw_children(OBJ_ATTR (*obj_attr_buffer)[], int obj_attr_num)override;

                    void input()override {}
                private:
                    unsigned short m_attr0;
                    unsigned short m_attr1;
                    unsigned short m_attr2;

                    core::gfx::Vector2 m_position;
            };
        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
