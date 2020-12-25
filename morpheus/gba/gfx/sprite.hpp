//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_SPRITE_HPP
#define MORPHEUS_SPRITE_HPP

#include <cassert>
#include <cstring>

#include <tonc.h>

#include <core/node.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite : public core::Node {
                public:
                    explicit Sprite(bool is_4bpp) {
                        m_is_4bpp = is_4bpp;
                    }

                    core::gfx::Vector2 get_position() const {
                        return m_position;
                    }

                    void set_position(const core::gfx::Vector2 position) {
                        m_position = position;
                    }

                    void set_position(const int x, const int y) {
                        set_position(core::gfx::Vector2(x, y));
                    }

                    void load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                         const unsigned short width, const unsigned short height,
                                         const unsigned short tile_id);

                    void load_from_array(const unsigned short *tile_array, const unsigned short width,
                                         const unsigned short height, const unsigned short tile_id);
                protected:
                    void build_attr2(const unsigned short palette_id, const unsigned short tile_id) {
                        m_attr2 = ATTR2_BUILD(tile_id, palette_id, 0);
                    }

                    void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority)override;

                    virtual void input(core::InputEvent input_event)override {}

                    virtual void array_load(const unsigned short *tile_array, const unsigned short width,
                                            const unsigned short height, const unsigned short tile_id) = 0;
                    virtual void array_load(const unsigned short *tile_array, const unsigned short *palette,
                                            const unsigned short width, const unsigned short height,
                                            const unsigned short tile_id) = 0;
                private:
                    bool m_is_4bpp;

                    unsigned short m_attr0;
                    unsigned short m_attr1;
                    unsigned short m_attr2;

                    core::gfx::Vector2 m_position;

                    void setup_size_attr(const unsigned short width, const unsigned short height);
            };
        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
