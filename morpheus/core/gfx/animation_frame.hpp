//
// Created by bobby on 26/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_ANIMATION_FRAME_HPP
#define MORPHEUS_GBA_TEST_ANIMATION_FRAME_HPP

#include <unordered_set>

#include <core/gfx/sprite_base.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class AnimationFrameCopyOption {
                BLENDING,
                SPRITE_SIZE,
                MOSAIC,
                PALETTE,
                POSITION,
                ROTATION,
                SCALE,
                VISIBLE,
                // for psuedo-inheritance purposes
                LAST
            };

            class AnimationFrame {
            public:
                AnimationFrame(core::gfx::SpriteBase *target_sprite, AnimationFrame *from_animation_frame);

                virtual ~AnimationFrame() = default;

                unsigned int get_blending_value() const {
                    return m_blending_value;
                }

                core::gfx::Vector2 get_mosaic_levels() const {
                    return m_mosaic_levels;
                }

                int get_rotation() const {
                    return m_rotation;
                }

                core::gfx::Vector2 get_position() const {
                    return m_position;
                }

                core::gfx::Vector2 get_scale() const {
                    return m_scale;
                }

                unsigned int get_vblank_delays() const {
                    return m_vblank_delays;
                }

                bool is_visible() const {
                    return m_visible;
                }

                void set_blending_value(unsigned int blending_value, bool enable_copy = true) {
                    m_blending_value = blending_value;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::BLENDING);
                    }
                }

                void set_mosaic_levels(core::gfx::Vector2 mosaic_levels, bool enable_copy = true) {
                    m_mosaic_levels = mosaic_levels;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::MOSAIC);
                    }
                }

                // TODO(Bobby): Implement Palette functions with Palette support

                void set_position(core::gfx::Vector2 position, bool enable_copy = true) {
                    m_position = position;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::POSITION);
                    }
                }

                void set_rotation(int rotation, bool enable_copy = true) {
                    m_rotation = rotation;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::ROTATION);
                    }
                }

                void set_scale(core::gfx::Vector2 scale, bool enable_copy = true) {
                    m_scale = scale;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::SCALE);
                    }
                }

                void set_sprite_size(core::gfx::SpriteSize sprite_size, bool enable_copy = true) {
                    m_sprite_size = sprite_size;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::SPRITE_SIZE);
                    }
                }

                void set_vblank_delays(const unsigned int vblank_delays) {
                    m_vblank_delays = vblank_delays;
                }

                void set_visible(bool visible, bool enable_copy = true) {
                    m_visible = visible;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::VISIBLE);
                    }
                }

                void enable_copy_option(AnimationFrameCopyOption copy_option) {
                    m_animation_frame_copy_options.emplace(copy_option);
                }

                void disable_copy_option(AnimationFrameCopyOption copy_option) {
                    m_animation_frame_copy_options.erase(copy_option);
                }

                bool is_copy_option_active(AnimationFrameCopyOption copy_option) {
                    return m_animation_frame_copy_options.find(copy_option) != m_animation_frame_copy_options.end();
                }

                void activate_on_target_sprite_base();
            protected:
                core::gfx::SpriteBase *get_target_sprite() const {
                    return m_target_sprite;
                }

                virtual void activate_on_target_sprite(AnimationFrameCopyOption copy_option) = 0;
            private:
                std::unordered_set<AnimationFrameCopyOption> m_animation_frame_copy_options;
                unsigned int m_blending_value;
                core::gfx::Vector2 m_mosaic_levels = core::gfx::Vector2(1, 1);
                unsigned int m_palette_id;
                core::gfx::Vector2 m_position;
                int m_rotation = 0;
                core::gfx::Vector2 m_scale = core::gfx::Vector2(1 << 8, 1 << 8);
                core::gfx::SpriteSize m_sprite_size;
                core::gfx::SpriteBase *m_target_sprite;
                bool m_visible = true;

                unsigned int m_vblank_delays;
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_ANIMATION_FRAME_HPP
