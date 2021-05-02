//
// Created by bobby on 28/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP
#define MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP

//#include <tonc.h>

#include <core/gfx/animation_frame.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            class IntegerAnimationSmoothingAttribute {
            public:
                IntegerAnimationSmoothingAttribute(core::gfx::SpriteBase *target_sprite,
                                                   core::gfx::AnimationFrameCopyOption copy_option,
                                                   int from, int to, unsigned int in_vblanks);

                virtual ~IntegerAnimationSmoothingAttribute() = default;

                // should be only called once per each VBlank
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

            class Vector2SmoothingAttribute : public IntegerAnimationSmoothingAttribute {
            public:
                Vector2SmoothingAttribute(core::gfx::SpriteBase *target_sprite,
                                          core::gfx::AnimationFrameCopyOption copy_option,
                                          core::gfx::Vector2 from, core::gfx::Vector2 to, unsigned int in_vblanks);

                ~Vector2SmoothingAttribute() override = default;

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
        }
    }
}


#endif //MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP
