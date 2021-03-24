//
// Created by bobby on 22/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_BASE_HPP
#define MORPHEUS_GBA_TEST_SPRITE_BASE_HPP

#include <core/controllers.hpp>
#include <core/node.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class SpriteSize {
                // square sprite sizes (x == y)
                SIZE_8X8,
                SIZE_16X16,
                SIZE_32X32,
                SIZE_64X64,

                // wide sprite sizes (x == 2 * y)
                SIZE_16X8,
                SIZE_32X16,
                SIZE_64X32,

                // long sprite sizes (2 * x == y)
                SIZE_8X16,
                SIZE_16X32,
                SIZE_32X64
            };

            class SpriteBase : public morpheus::core::Node {
            public:
                SpriteBase(BlendingController *blending_controller) {
                    m_blending_controller = blending_controller;
                }

                virtual ~SpriteBase() {}

                unsigned int get_priority() const {
                    return m_priority;
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

                void set_priority(unsigned int priority) {
                    m_priority = std::min(3u, priority);
                }

                virtual bool is_blended() const = 0;

                void disable_blending() {
                    if(m_blending_controller != nullptr) {
                        toggle_blending(false);
                    }
                }
                void enable_blending(bool bottom_layer) {
                    if(m_blending_controller != nullptr) {
                        toggle_blending(true, bottom_layer);
                    }
                }

                virtual bool load_into_palette(const unsigned short *palette, const unsigned int pal_len) = 0;
            protected:
                virtual void toggle_blending(bool enable_blending, bool bottom_layer = true) = 0;

                BlendingController *get_blending_controller() const {
                    return m_blending_controller;
                }

                virtual void set_sprite_size(SpriteSize size) = 0;
            private:
                BlendingController *m_blending_controller;
                Vector2 m_position;
                unsigned char m_priority = 0;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_BASE_HPP
