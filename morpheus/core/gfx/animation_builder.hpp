//
// Created by bobby on 24/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_ANIMATION_BUILDER_HPP
#define MORPHEUS_GBA_TEST_ANIMATION_BUILDER_HPP

#include <core/gfx/sprite_base.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {

            class AnimationBuilder {
            public:
                explicit AnimationBuilder(core::gfx::SpriteBase *from);

                std::vector<AnimationFrame> get_animation_frames() const {
                    return m_animation_frames;
                }

                bool remove_frame(unsigned int i) {
                    if(m_animation_frames.size() > 1 && m_animation_frames.size() > i) {
                        m_animation_frames.erase(m_animation_frames.begin() + static_cast<int>(i));

                        return true;
                    } else {
                        return false;
                    }
                }
            protected:
                std::vector<AnimationFrame> m_animation_frames;

                void copy_appropriate_frame_attribute(morpheus::core::gfx::AnimationFrame &dest,
                                                      morpheus::core::gfx::AnimationFrame source,
                                                      AnimationFrameCopyOption copy_option);
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_ANIMATION_BUILDER_HPP
