//
// Created by bobby on 26/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_ANIMATION_FRAME_HPP
#define MORPHEUS_GBA_TEST_ANIMATION_FRAME_HPP

#include <map>
#include <unordered_set>

#include "sprite_base.hpp"
#include "vector_2.hpp"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class AnimationFrameCopyOption {
                BLENDING, ///< SpriteBase blending levels/BlendingSetting
                SPRITE_SIZE, ///< SpriteBase SpriteSize
                MOSAIC, ///< SpriteBase mosaic levels
                PALETTE, ///< SpriteBase palette number
                POSITION, ///< SpriteBase position
                ROTATION, ///< Affine SpriteBase rotation
                SCALE, ///< Affine SpriteBase scale
                VISIBLE, ///< SpriteBase Visibility
                LAST ///< Exclusively used for psuedo-inheritance purposes
            };


            /// \enum morpheus::core::gfx::AnimationFrameCopyOption
            ///
            /// An enum class consisting of all the sprite attributes that can
            /// be modified by the animation engine on both the GBA and the DS.
            /// The sole exception is AnimationFrameCopyOption::LAST, which
            /// is used for psuedo-inheritance purposes with
            /// gba::gfx::GbaAnimationFrameCopyOption and
            /// nds::gfx::NdsAnimationFrameCopyOption.

            enum class AnimationSmoothingMode {
                NONE, ///< No animation smoothing applied
                LINEAR ///< Linear animation smoothin applied
            };


            /// \enum morpheus::core::gfx::AnimationSmoothingMode
            ///
            /// An enum class consisting of all the smoothing modes that can be
            /// used on attributes within their animation frames.
            /// Currently only linear smoothing or smoothing is supported on all
            /// attributes except for AnimationFrameCopyOption::PALETTE and
            /// AnimationFrameCopyOption::VISIBLE.

            class AnimationFrame {
            public:
                /// Constructs an AnimationFrame to be used by a SpriteBase
                /// through the SpriteBase::set_frames() function.
                /// This must have a SpriteBase to conduct this animation on
                /// and may have another AnimationFrame to construct itself from.
                /// \param target_sprite The SpriteBase to conduct this animation on
                /// \param from_animation_frame The previous AnimationFrame (if any)
                /// to construct this animation from.
                AnimationFrame(SpriteBase *target_sprite, AnimationFrame *from_animation_frame = nullptr);

                /// Destructs an AnimationFrame.
                virtual ~AnimationFrame() = default;

                /// The values from these SpriteBase attribute
                /// getters will not be used unless they are explicitly
                /// enabled through the enable_copy argument being set to true
                /// on the respective SpriteBase attribute setters.
                /// <br><br>
                /// In addition, remeber to set the BlendingMode on the
                /// SpriteBase to the desired effect before starting a
                /// blending animation.
                /// \return the blending value specified by this AnimationFrame.
                unsigned int get_blending_value() const {
                    return m_blending_value;
                }

                /// The values from these SpriteBase attribute
                /// getters will not be used unless they are explicitly
                /// enabled through the enable_copy argument being set to true
                /// on the respective SpriteBase attribute setters.
                /// \return the mosaic levels specified by this AnimationFrame.
                core::gfx::Vector2 get_mosaic_levels() const {
                    return m_mosaic_levels;
                }

                /// The values from these SpriteBase attribute
                /// getters will not be used unless they are explicitly
                /// enabled through the enable_copy argument being set to true
                /// on the respective SpriteBase attribute setters.
                /// <br><br>
                /// In addition, make sure that the SpriteBase is affine before
                /// trying to do an affine animation.
                /// \return The fixed-point rotation specified by this AnimationFrame
                int get_rotation() const {
                    return m_rotation;
                }

                /// The values from these SpriteBase attribute
                /// getters will not be used unless they are explicitly
                /// enabled through the enable_copy argument being set to true
                /// on the respective SpriteBase attribute setters.
                /// \return The position specified by this AnimationFrame
                core::gfx::Vector2 get_position() const {
                    return m_position;
                }

                /// The values from these SpriteBase attribute
                /// getters will not be used unless they are explicitly
                /// enabled through the enable_copy argument being set to true
                /// on the respective SpriteBase attribute setters.
                /// <br><br>
                /// In addition, make sure that the SpriteBase is affine before
                /// trying to do an affine animation.
                /// \return The fixed-point scale specified by this AnimationFrame
                core::gfx::Vector2 get_scale() const {
                    return m_scale;
                }

                /// \return The number of VBlanks to wait until this AnimationFrame should be
                /// fully triggered since the last AnimationFrame
                unsigned int get_vblank_delays() const {
                    return m_vblank_delays;
                }

                /// The values from these SpriteBase attribute
                /// getters will not be used unless they are explicitly
                /// enabled through the enable_copy argument being set to true
                /// on the respective SpriteBase attribute setters.
                /// <br><br>
                /// In addition, this attribute cannot be smoothed.
                /// \return Whether the SpriteBase is visible in this AnimationFrame
                bool is_visible() const {
                    return m_visible;
                }

                /// Sets the blending value used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is set
                /// to true. Remeber to set the BlendingMode on this SpriteBase
                /// to the desired effect before starting a blending animation.
                /// \param blending_value The blending value to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void set_blending_value(unsigned int blending_value, bool enable_copy = true,
                                        AnimationSmoothingMode smoothing_mode = AnimationSmoothingMode::NONE) {
                    m_blending_value = blending_value;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::BLENDING, smoothing_mode);
                    }
                }

                /// Sets the mosaic levels used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is
                /// set to true.
                /// \param blending_value The mosaic levels to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void set_mosaic_levels(core::gfx::Vector2 mosaic_levels, bool enable_copy = true,
                                       AnimationSmoothingMode smoothing_mode = AnimationSmoothingMode::NONE) {
                    m_mosaic_levels = mosaic_levels;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::MOSAIC, smoothing_mode);
                    }
                }

                // TODO(Bobby): Implement Palette functions with Palette support

                /// Sets the position used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is
                /// set to true.
                /// \param blending_value The position to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void set_position(core::gfx::Vector2 position, bool enable_copy = true,
                                  AnimationSmoothingMode smoothing_mode = AnimationSmoothingMode::NONE) {
                    m_position = position;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::POSITION, smoothing_mode);
                    }
                }

                /// Sets the rotation used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is
                /// set to true. Remeber to make sure that the SpriteBase is
                /// affine before starting an animation on an affine attribute.
                /// \param blending_value The rotation to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void set_rotation(int rotation, bool enable_copy = true,
                                  AnimationSmoothingMode smoothing_mode = AnimationSmoothingMode::NONE) {
                    m_rotation = rotation;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::ROTATION, smoothing_mode);
                    }
                }

                /// Sets the scale used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is
                /// set to true. Remeber to make sure that the SpriteBase is
                /// affine before starting an animation on an affine attribute.
                /// \param blending_value The scale to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void set_scale(core::gfx::Vector2 scale, bool enable_copy = true,
                               AnimationSmoothingMode smoothing_mode = AnimationSmoothingMode::NONE) {
                    m_scale = scale;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::SCALE, smoothing_mode);
                    }
                }

                /// Sets the sprite size used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is
                /// set to true.
                /// \param sprite_size The sprite size to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void set_sprite_size(core::gfx::SpriteSize sprite_size, bool enable_copy = true,
                                     AnimationSmoothingMode smoothing_mode = AnimationSmoothingMode::NONE) {
                    m_sprite_size = sprite_size;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::SPRITE_SIZE, smoothing_mode);
                    }
                }

                /// Sets the number of VBlanks to wait until this AnimationFrame should be
                /// fully triggered since the last AnimationFrame.
                /// \param vblank_delays The number of VBlanks to use
                void set_vblank_delays(const unsigned int vblank_delays) {
                    m_vblank_delays = vblank_delays;
                }

                /// Sets the sprite visiblity used by this AnimationFrame and
                /// that is applied to the SpriteBase if enable_copy is
                /// set to true.
                /// \param visible The sprite visibilty (true for visible,
                /// false for invisible) to apply
                /// \param enable_copy Whether to enable copying this attribute
                /// to the SpriteBase upon this AnimationFrame becoming active
                void set_visible(bool visible, bool enable_copy = true) {
                    m_visible = visible;

                    if(enable_copy) {
                        enable_copy_option(AnimationFrameCopyOption::VISIBLE,
                                           AnimationSmoothingMode::NONE);
                    }
                }

                /// Enables a sprite attribute with a certain smoothing mode
                /// to be copied to the SpriteBase upon this AnimationFrame getting fully triggered.
                /// \param copy_option Which sprite attribute to enable the
                /// copying of
                /// \param smoothing_mode The AnimationSmoothingMode to smooth
                /// the transition from this AnimationFrame attribute to the
                /// next AnimationFrame attribute.
                void enable_copy_option(AnimationFrameCopyOption copy_option, AnimationSmoothingMode smoothing_mode) {
                    m_animation_frame_copy_options.emplace(copy_option);
                    m_animation_smoothing_modes[copy_option] = smoothing_mode;
                }

                /// Disables a sprite attribute from being copied to the
                /// SpriteBase upon this AnimationFrame getting fully
                /// triggered.
                /// \param copy_option Which sprite attribute to disable the
                /// copying of
                void disable_copy_option(AnimationFrameCopyOption copy_option) {
                    m_animation_frame_copy_options.erase(copy_option);
                    m_animation_smoothing_modes.erase(copy_option);
                }

                /// \param copy_option
                /// \return Whether a certain sprite attribute will be copied
                /// to the SpriteBase upon this AnimationFrame getting fully
                /// triggered
                bool is_copy_option_active(AnimationFrameCopyOption copy_option) const {
                    return m_animation_frame_copy_options.find(copy_option) != m_animation_frame_copy_options.end();
                }

                std::unordered_set<AnimationFrameCopyOption> get_copy_options_with_smoothing_mode(
                                                                        AnimationSmoothingMode smoothing_mode)  {
                    std::unordered_set<AnimationFrameCopyOption> return_value;

                    for(AnimationFrameCopyOption copy_option : m_animation_frame_copy_options) {
                        if(m_animation_smoothing_modes[copy_option] == smoothing_mode) {
                            return_value.emplace(copy_option);
                        }
                    }

                    return return_value;
                }

                /// \return A string that contains the number of enabled AnimationFrameCopyOptions
                /// and the VBlank delay for easier debugging
                std::string to_string();

                /// Copies all the enabled sprite attributes to the SpriteBase
                /// that this frame is acting on. Note: This function should
                /// usually be called by the SpriteBase automatically and thus
                /// shouldn't be called be manually.
                void activate_on_target_sprite_base();
            protected:
                /// \return The target SpriteBase to use when applying this
                /// AnimationFrame's copyable attributes.
                core::gfx::SpriteBase *get_target_sprite() const {
                    return m_target_sprite;
                }

                /// Activates a certain copyable AnimationFrame attribute on the
                /// target SpriteBase in a platform-specific way upon this
                /// AnimationFrame becoming fully active.
                /// \param copy_option The AnimationFrameCopyOption for a
                /// specific AnimationFrame attribute.
                virtual void activate_on_target_sprite(AnimationFrameCopyOption copy_option) = 0;
            private:
                std::unordered_set<AnimationFrameCopyOption> m_animation_frame_copy_options;
                std::map<AnimationFrameCopyOption, AnimationSmoothingMode> m_animation_smoothing_modes;
                unsigned int m_blending_value;
                core::gfx::Vector2 m_mosaic_levels = core::gfx::Vector2(1, 1);
                unsigned int m_palette_id;
                core::gfx::Vector2 m_position;
                int m_rotation = 0;
                core::gfx::Vector2 m_scale = core::gfx::Vector2(1 << 8, 1 << 8);
                core::gfx::SpriteSize m_sprite_size;
                core::gfx::SpriteBase *m_target_sprite;
                bool m_visible = true;

                unsigned int m_vblank_delays = 0;
            };


            /// \class morpheus::core::gfx::AnimationFrame
            /// A class representing an animation frame that can be used on
            /// SpriteBases by creating a vector of the desired
            /// AnimationFrames in an animation, calling
            /// SpriteBase::set_frames() to set the frames, and then
            /// calling SpriteBase::play() to play the animation.
            /// For an example of this class being used, see
            /// ../tests/animation_test/animation_test.cpp and
            /// ../tests/animation_test/test_animation.hpp
        }
    }
}


#endif //MORPHEUS_GBA_TEST_ANIMATION_FRAME_HPP
