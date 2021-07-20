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
                GFX_PTR = static_cast<int>(core::gfx::AnimationFrameCopyOption::LAST) ///< NDS
                                                                                      ///< tile
                                                                                      ///< graphics
                                                                                      ///< pointer
                                                                                      ///< Sprite
                                                                                      ///< attribute
            };

            /// \enum morpheus::nds::gfx::NdsAnimationFrameCopyOption
            /// An enum class psuedo-inherited from
            /// morpheus::core::gfx::AnimationFrameCopyOption that provides
            /// copy options for NDS-specific Sprite attributes.

            class NdsAnimationFrame : public core::gfx::AnimationFrame {
            public:
                /// Constructs an NdsAnimationFrame for a certain Sprite and
                /// optionally copies attributes from a previous
                /// NdsAnimationFrame.
                /// \param target_sprite The Sprite for this NdsAnimationFrame
                /// to act on.
                /// \param from_animation_frame The optional previous
                /// NdsAnimationFrame to copy the attributes from on to this
                /// NdsAnimationFrame.
                explicit NdsAnimationFrame(nds::gfx::Sprite *target_sprite,
                                           NdsAnimationFrame *from_animation_frame = nullptr);

                /// Deconstructs the NdsAnimationFrame object.
                virtual ~NdsAnimationFrame();

                /// \return The NDS-specific tile graphics (gfx) pointer for
                /// this NdsAnimationFrame.
                uint16_t *get_gfx_pointer() const {
                    return m_gfx_pointer;
                }

                /// Sets the NDS-specific tile id used by this NdsAnimationFrame
                /// and that is applied to the Sprite if enable_copy is set
                /// to true.
                /// \param tile_id The NDS-specific tile graphics pointer to
                /// apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the Sprite upon this AnimationFrame becoming active
                /// \param smoothing_mode The
                /// morpheus::core::gfx::AnimationSmoothingMode to smooth
                /// the transition from this NdsAnimationFrame attribute to the
                /// next NdsAnimationFrame attribute.
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

            /// \class morpheus::nds::gfx::NdsAnimationFrame
            /// The Nintendo DS implementation of
            /// morpheus::core::gfx::AnimationFrame, with animation support for
            /// NDS-specific tile ids. For more about this class's
            /// behavior, see the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_ANIMATION_FRAME_HPP
