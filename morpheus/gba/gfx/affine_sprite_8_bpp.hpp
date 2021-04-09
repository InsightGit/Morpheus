//
// Created by bobby on 07/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_AFFINE_SPRITE_8_BPP_HPP
#define MORPHEUS_GBA_TEST_AFFINE_SPRITE_8_BPP_HPP

#include <core/gfx/affine_interface.hpp>

#include <gba/gfx/sprite_8_bpp.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class AffineSprite8Bpp : public Sprite8Bpp, public core::gfx::AffineInterface {
            public:
                AffineSprite8Bpp(GbaBlendingController *blending_controller, GbaMosaicController *mosaic_controller) :
                    Sprite8Bpp(blending_controller, mosaic_controller) {}

                int get_rotation() const override {
                    return m_rotation;
                }

                core::gfx::Vector2 get_scale() const override {
                    return m_scale;
                }

                void set_rotation(const int rotation) override {
                    m_rotation = rotation;
                }

                void set_scale(const core::gfx::Vector2 scale) override {
                    m_scale = scale;
                }
            protected:
                void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num)override;
            private:
                int m_rotation;
                core::gfx::Vector2 m_scale = core::gfx::Vector2(1, 1);
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_AFFINE_SPRITE_8_BPP_HPP
