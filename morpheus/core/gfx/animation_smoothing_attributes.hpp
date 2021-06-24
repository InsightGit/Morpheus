//
// Created by bobby on 28/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP
#define MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP

#include <core/gfx/animation_frame.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            class IntegerAnimationSmoothingAttribute {
            public:
                /// Constructs an IntegerSmoothingAttribute which will conduct
                /// linear smoothing on the transition between two
                /// AnimationFrames for a certain integer SpriteBase attribute
                /// in a certain amount of VBlanks. Note: this class is usually
                /// used only internally within SpriteBase::update_animation()
                /// and shouldn't need to be constructed manually.
                /// \param target_sprite The SpriteBase that the AnimationFrames
                /// are acting on
                /// \param copy_option The SpriteBase integer attribute to act on
                /// \param from The value of the SpriteBase integer attribute
                /// upon the animation smoothing starting
                /// \param to The value of the SpriteBase integer attribute upon
                /// the animation smoothing finishing
                /// \param in_vblanks The amount of VBlanks for the smoothed
                /// animation transition to take
                IntegerAnimationSmoothingAttribute(core::gfx::SpriteBase *target_sprite,
                                                   core::gfx::AnimationFrameCopyOption copy_option,
                                                   int from, int to, unsigned int in_vblanks);

                /// Destructs an IntegerAnimationSmoothingAttribute.
                virtual ~IntegerAnimationSmoothingAttribute() = default;

                /// Conducts one VBlank's worth of smoothing on the
                /// integer SpriteBase attribute and applies it to the
                /// SpriteBase.
                /// This function will return false if it cannot smooth the
                /// specified AnimationFrameCopyOption.
                /// Note: This function should ONLY be called once per each
                /// VBlank or else the animation smoothing will no longer be
                /// in sync.
                /// \return Whether the smoothing was successful or not
                virtual bool smooth();
            protected:
                core::gfx::AnimationFrameCopyOption get_copy_option() const {
                    return m_copy_option;
                }

                core::gfx::SpriteBase *get_target_sprite() const {
                    return m_target_sprite;
                }
            private:
                int get_min_value(int value) const {
                    if(m_decreasing) {
                        return std::max(m_goal, value);
                    } else {
                        return std::min(m_goal, value);
                    }
                }

                core::gfx::AnimationFrameCopyOption m_copy_option;
                bool m_decreasing;
                int m_compensation;
                int m_frame_counter = 0;
                int m_goal;
                core::gfx::SpriteBase *m_target_sprite;
                int m_trend = 0;
            };


            /// \class morpheus::core::gfx::IntegerAnimationSmoothingAttribute
            /// A helper class for morpheus::core::gfx::SpriteBase which
            /// will conduct linear smoothing on the transition between two
            /// AnimationFrames for a certain integer SpriteBase attribute in a
            /// certain amount of VBlanks. Note: this class is usually used
            /// only internally within SpriteBase::update_animation() and
            /// shouldn't need to be constructed manually.

            class Vector2SmoothingAttribute : public IntegerAnimationSmoothingAttribute {
            public:
                /// Constructs an Vector2SmoothingAttribute which will conduct
                /// linear smoothing on the transition between two
                /// AnimationFrames for a certain Vector2 SpriteBase attribute
                /// in a certain amount of VBlanks. Note: this class is usually
                /// used only internally within SpriteBase::update_animation()
                /// and shouldn't need to be constructed manually.
                /// \param target_sprite The SpriteBase that the AnimationFrames
                /// are acting on
                /// \param copy_option The SpriteBase Vector2 attribute to act
                /// on
                /// \param from The value of the SpriteBase Vector2 attribute
                /// upon the animation smoothing starting
                /// \param to The value of the SpriteBase Vector2 attribute upon
                /// the animation smoothing finishing
                /// \param in_vblanks The amount of VBlanks for the smoothed
                /// animation transition to take
                Vector2SmoothingAttribute(core::gfx::SpriteBase *target_sprite,
                                          core::gfx::AnimationFrameCopyOption copy_option,
                                          core::gfx::Vector2 from, core::gfx::Vector2 to, unsigned int in_vblanks);

                /// Destructs a Vector2SmoothingAttribute.
                ~Vector2SmoothingAttribute() override = default;

                /// Conducts one VBlank's worth of smoothing on the
                /// Vector2 SpriteBase attribute and applies it to the
                /// SpriteBase.
                /// This function will return false if it cannot smooth the
                /// specified AnimationFrameCopyOption.
                /// Note: This function should ONLY be called once per each
                /// VBlank or else the animation smoothing will no longer be
                /// in sync.
                /// \return Whether the smoothing was successful or not
                bool smooth()override;
            private:
                core::gfx::Vector2 get_min_vector(core::gfx::Vector2 vector);

                // explicit is better than implicit
                bool m_decreasing_x;
                bool m_decreasing_y;
                core::gfx::Vector2 m_vector_frame_counter;
                core::gfx::Vector2 m_vector_goal;
                core::gfx::Vector2 m_vector_trend;
                //core::gfx::Vector2 m_vector_trend_per_frames = core::gfx::Vector2(1, 1);
                core::gfx::Vector2 m_vector_compensation;
            };


            /// \class morpheus::core::gfx::Vector2AnimationSmoothingAttribute
            /// A helper class for morpheus::core::gfx::SpriteBase which
            /// will conduct linear smoothing on the transition between two
            /// AnimationFrames for a certain Vector2 SpriteBase attribute in a
            /// certain amount of VBlanks. Note: this class is usually used
            /// only internally within SpriteBase::update_animation() and
            /// shouldn't need to be constructed manually.
        }
    }
}


#endif //MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP
