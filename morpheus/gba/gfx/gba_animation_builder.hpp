//
// Created by bobby on 25/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_ANIMATION_BUILDER_HPP
#define MORPHEUS_GBA_TEST_GBA_ANIMATION_BUILDER_HPP

#include <map>

#include <core/gfx/animation_builder.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite;

            class GbaAnimationBuilder : public core::gfx::AnimationBuilder {
            public:
                GbaAnimationBuilder(gba::gfx::Sprite *from);

                void add_frame(GbaAnimationFrame frame, std::vector<GbaAnimationFrameCopyOption> copy_options);
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_ANIMATION_BUILDER_HPP
