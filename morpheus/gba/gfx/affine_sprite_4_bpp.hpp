//
// Created by bobby on 05/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_AFFINE_SPRITE_4_BPP_HPP
#define MORPHEUS_GBA_TEST_AFFINE_SPRITE_4_BPP_HPP

#include <core/gfx/affine_interface.hpp>

#include <gba/gfx/sprite_4_bpp.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class AffineSprite4Bpp : public Sprite4Bpp, public core::gfx::AffineInterface {
            public:
                AffineSprite4Bpp(GbaBlendingController *blending_controller, GbaMosaicController *mosaic_controller,
                                 const unsigned int palette_id = 0) : Sprite4Bpp(blending_controller, mosaic_controller,
                                                                                 palette_id) {}
                AffineSprite4Bpp(GbaBlendingController *blending_controller, GbaMosaicController *mosaic_controller,
                                 const unsigned int tile_id, const unsigned int palette_id,
                                 const core::gfx::SpriteSize size) : Sprite4Bpp(blending_controller, mosaic_controller,
                                                                                tile_id, palette_id, size) {}

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
                int m_rotation = 0;
                core::gfx::Vector2 m_scale = core::gfx::Vector2(1, 1);
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_AFFINE_SPRITE_4_BPP_HPP
