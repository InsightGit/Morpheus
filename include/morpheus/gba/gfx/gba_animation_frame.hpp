//
// Created by bobby on 26/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_ANIMATION_FRAME_HPP
#define MORPHEUS_GBA_TEST_GBA_ANIMATION_FRAME_HPP

#include <morpheus/core/gfx/animation_frame.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite;

            enum class GbaAnimationFrameCopyOption {
                TILE_ID = static_cast<int>(core::gfx::AnimationFrameCopyOption::LAST) ///< GBA
                                                                                      ///< tile
                                                                                      ///< id
                                                                                      ///< Sprite
                                                                                      ///< attribute
            };

            /// \enum morpheus::gba::gfx::GbaAnimationFrameCopyOption
            /// An enum class psuedo-inherited from
            /// morpheus::core::gfx::AnimationFrameCopyOption that provides
            /// copy options for GBA-specific Sprite attributes.

            class GbaAnimationFrame : public core::gfx::AnimationFrame {
            public:
                /// Constructs a GbaAnimationFrame for a certain Sprite and
                /// optionally copies attributes from a previous
                /// GbaAnimationFrame.
                /// \param target_sprite The Sprite for this GbaAnimationFrame
                /// to act on.
                /// \param from_animation_frame The optional previous
                /// GbaAnimationFrame to copy the attributes from on to this
                /// GbaAnimationFrame.
                explicit GbaAnimationFrame(gba::gfx::Sprite *target_sprite,
                                           GbaAnimationFrame *from_animation_frame = nullptr);

                /// Destructs the GbaAnimationFrame object.
                ~GbaAnimationFrame() override = default;

                /// \return The GBA-specific tile id attribute for this
                /// GbaAnimationFrame.
                unsigned int get_tile_id() const {
                    return m_tile_id;
                }

                /// Sets the GBA-specific tile id used by this GbaAnimationFrame
                /// and that is applied to the Sprite if enable_copy is set
                /// to true.
                /// \param tile_id The GBA-specific tile id to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the Sprite upon this AnimationFrame becoming active
                /// \param smoothing_mode The
                /// morpheus::core::gfx::AnimationSmoothingMode to smooth
                /// the transition from this GbaAnimationFrame attribute to the
                /// next GbaAnimationFrame attribute.
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

            /// \class morpheus::gba::gfx::GbaAnimationFrame
            /// The Game Boy Advance implementation of
            /// morpheus::core::gfx::AnimationFrame, with animation support for
            /// GBA-specific tile ids. For more about this class's
            /// behavior, see the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_ANIMATION_FRAME_HPP
