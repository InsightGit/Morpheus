//
// Created by bobby on 26/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_ANIMATION_FRAME_HPP
#define MORPHEUS_GBA_TEST_GBA_ANIMATION_FRAME_HPP

#include <core/gfx/animation_frame.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite;

            enum class GbaAnimationFrameCopyOption {
                TILE_ID = static_cast<int>(core::gfx::AnimationFrameCopyOption::LAST)
            };

            class GbaAnimationFrame : public core::gfx::AnimationFrame {
            public:
                explicit GbaAnimationFrame(gba::gfx::Sprite *target_sprite,
                                           GbaAnimationFrame *from_animation_frame = nullptr);

                ~GbaAnimationFrame() override = default;

                unsigned int get_tile_id() const {
                    return m_tile_id;
                }

                void set_tile_id(const unsigned int tile_id,
                                 core::gfx::AnimationSmoothingMode smoothing_mode =
                                                                                core::gfx::AnimationSmoothingMode::NONE,
                                 bool enable_copy = true) {
                    m_tile_id = tile_id;

                    if(enable_copy) {
                        enable_copy_option(
                                static_cast<core::gfx::AnimationFrameCopyOption>(GbaAnimationFrameCopyOption::TILE_ID),
                                smoothing_mode);
                    }
                }

                void activate_on_target_sprite(core::gfx::AnimationFrameCopyOption copy_option)override;
            private:
                unsigned int m_tile_id;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_ANIMATION_FRAME_HPP
