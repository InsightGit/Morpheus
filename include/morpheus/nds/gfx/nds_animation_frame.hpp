//
// Created by bobby on 01/05/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_ANIMATION_FRAME_HPP
#define MORPHEUS_GBA_TEST_NDS_ANIMATION_FRAME_HPP

#include <morpheus/core/gfx/animation_frame.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class Sprite;

            enum class NdsAnimationFrameCopyOption {
                GFX_PTR = static_cast<int>(core::gfx::AnimationFrameCopyOption::LAST)
            };

            class NdsAnimationFrame : public core::gfx::AnimationFrame {
            public:
                explicit NdsAnimationFrame(nds::gfx::Sprite *target_sprite,
                                           NdsAnimationFrame *from_animation_frame = nullptr);

                virtual ~NdsAnimationFrame();

                uint16_t *get_gfx_pointer() const {
                    return m_gfx_pointer;
                }

                void set_gfx_pointer(uint16_t *gfx_pointer,
                                     core::gfx::AnimationSmoothingMode smoothing_mode =
                                     core::gfx::AnimationSmoothingMode::NONE,
                                     bool enable_copy = true) {
                    m_gfx_pointer = gfx_pointer;

                    if(enable_copy) {
                        enable_copy_option(
                                static_cast<core::gfx::AnimationFrameCopyOption>(NdsAnimationFrameCopyOption::GFX_PTR),
                                smoothing_mode);
                    }
                }

                void activate_on_target_sprite(core::gfx::AnimationFrameCopyOption copy_option)override;
            private:
                uint16_t *m_gfx_pointer = nullptr;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_ANIMATION_FRAME_HPP
