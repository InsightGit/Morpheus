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
                IntegerAnimationSmoothingAttribute(core::gfx::SpriteBase *target_sprite,
                                                   core::gfx::AnimationFrameCopyOption copy_option,
                                                   int from, int to, unsigned int in_vblanks) {
                    m_copy_option = copy_option;
                    m_target_sprite = target_sprite;

                    if(to - from != 0) {
                        while(m_trend == 0) {
                            m_trend = (m_trend_per_frames * (to - from)) / static_cast<int>(in_vblanks);

                            if(m_trend == 0) {
                                m_trend_per_frames *= 2;
                            }
                        }
                    }
                }

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
                core::gfx::AnimationFrameCopyOption m_copy_option;
                int m_frame_count = 0;
                core::gfx::SpriteBase *m_target_sprite;
                int m_trend = 0;
                int m_trend_per_frames = 1;
            };

            class Vector2SmoothingAttribute : public IntegerAnimationSmoothingAttribute {
            public:
                Vector2SmoothingAttribute(core::gfx::SpriteBase *target_sprite,
                                          core::gfx::AnimationFrameCopyOption copy_option,
                                          core::gfx::Vector2 from, core::gfx::Vector2 to, unsigned int in_vblanks) :
                        IntegerAnimationSmoothingAttribute(target_sprite, copy_option, 0, 0, 0) {
                    while(m_vector_trend.get_x() == 0 || m_vector_trend.get_y() == 0) {
                        if(m_vector_trend.get_x() == 0) {
                            m_vector_trend = core::gfx::Vector2(
                                    (m_vector_trend_per_frames.get_x() * (to.get_x() - from.get_x())) /
                                        static_cast<int>(in_vblanks), m_vector_trend.get_y());

                            m_vector_trend_per_frames = core::gfx::Vector2(m_vector_trend_per_frames.get_x() * 2,
                                                                           m_vector_trend_per_frames.get_y());
                        }

                        if(m_vector_trend.get_y() == 0) {
                            m_vector_trend = core::gfx::Vector2(
                                    m_vector_trend.get_x(),
                                    (m_vector_trend_per_frames.get_y() * (to.get_y() - from.get_y())) /
                                    static_cast<int>(in_vblanks));

                            m_vector_trend_per_frames = core::gfx::Vector2(m_vector_trend_per_frames.get_x(),
                                                                           m_vector_trend_per_frames.get_y() * 2);
                        }
                    }
                }

                ~Vector2SmoothingAttribute() override = default;

                bool smooth()override;
            private:
                // explicit is better than implicit
                core::gfx::Vector2 m_vector_frame_count = core::gfx::Vector2(0, 0);
                core::gfx::Vector2 m_vector_trend = core::gfx::Vector2(0, 0);
                core::gfx::Vector2 m_vector_trend_per_frames = core::gfx::Vector2(1, 1);
            };
        }
    }
}


#endif //MORPHEUS_GBA_TEST_ANIMATION_SMOOTHING_ATTRIBUTES_HPP
