//
// Created by bobby on 27/11/2020.
//

/*#ifndef MORPHEUS_NDS_SPRITE_HPP
#define MORPHEUS_NDS_SPRITE_HPP

#include <core/node.hpp>

#include <nds.h>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite : public core::Node {
                public:
                    Sprite() = default;

                    core::gfx::Vector2 get_position() const {
                        return m_position;
                    }

                    void set_position(const core::gfx::Vector2 position) {
                        m_position = position;
                    }

                    void set_position(const int x, const int y) {
                        set_position(core::gfx::Vector2(x, y));
                    }

                    bool load_from_pcx(const unsigned char *pcx_data);
                protected:
                    void draw_children();

                    virtual void input(core::InputEvent input_event)override {}
                private:
                    std::unique_ptr<sImage> m_sprite_image;

                    core::gfx::Vector2 m_position;
            };
        }
    }
}

#endif //MORPHEUS_NDS_SPRITE_HPP*/
