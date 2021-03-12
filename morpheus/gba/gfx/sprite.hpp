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

#include <gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite : public core::Node {
                public:
                    explicit Sprite(const bool is_4bpp) {
                        m_is_4bpp = is_4bpp;
                    }

                    explicit Sprite(const bool is_4bpp, const unsigned short palette_id, const unsigned short tile_id,
                                    const unsigned short width, const unsigned short height) : Sprite(is_4bpp) {
                        build_attr2(palette_id, tile_id);
                        setup_size_attr(width, height);
                    }

                    virtual ~Sprite();

                    core::gfx::Vector2 get_position() const {
                        return m_position;
                    }

                    unsigned int get_priority() const {
                        return m_priority;
                    }

                    void set_position(const core::gfx::Vector2 position) {
                        m_position = position;
                    }

                    void set_position(const int x, const int y) {
                        set_position(core::gfx::Vector2(x, y));
                    }

                    void set_priority(unsigned int priority) {
                        m_priority = std::min(3u, priority);
                    }

                    void load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                         const unsigned int palette_len, const unsigned int width,
                                         const unsigned int height, const unsigned int tile_id);

                    void load_from_array(const unsigned short *tile_array, const unsigned int width,
                                         const unsigned int height, const unsigned int tile_id);

                    virtual void load_into_palette(const unsigned short *palette, const unsigned int palette_len) = 0;
                protected:
                    void build_attr2(const unsigned short palette_id, const unsigned short tile_id) {
                        m_attr2 = ATTR2_BUILD(tile_id, palette_id, 0);
                    }
                    void setup_size_attr(const unsigned short width, const unsigned short height);

                    void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority)override;
                    void on_visible_state_changed(bool hidden)override;

                    virtual void input(core::InputEvent input_event)override {}
                    virtual void update(unsigned char cycle_time)override {}

                    virtual void array_load(const unsigned short *tile_array, const unsigned int width,
                                            const unsigned int height, const unsigned int tile_id) = 0;
                    virtual void array_load(const unsigned short *tile_array, const unsigned short *palette,
                                            const unsigned int palette_len, const unsigned int width,
                                            const unsigned int height, const unsigned int tile_id) = 0;
                private:
                    bool m_is_4bpp;
                    int m_last_obj_attr_num;
                    unsigned int m_priority = 0;

                    unsigned short m_attr0;
                    unsigned short m_attr1;
                    unsigned short m_attr2;

                    core::gfx::Vector2 m_position;
            };
        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
