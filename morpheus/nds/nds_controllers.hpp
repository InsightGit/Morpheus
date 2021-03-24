//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_NDS_CONTROLLERS_HPP
#define MORPHEUS_GBA_TEST_NDS_CONTROLLERS_HPP

#include <nds.h>

#include <core/core.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class NdsBlendingController : public morpheus::core::gfx::BlendingController {
            public:
                NdsBlendingController(bool use_sub_display) {
                    m_use_sub_display = use_sub_display;
                }

                virtual ~NdsBlendingController() {}

                void disable_backdrop_blending()override;
                void enable_backdrop_blending(bool bottom)override;

                void disable_background_blending(unsigned int background)override;
                void enable_background_blending(bool bottom, unsigned int background)override;

                void disable_object_blending()override;
                void enable_object_blending(bool bottom)override;

                unsigned char get_blend_weight(bool bottom)const override;
                void set_blend_weight(bool bottom, unsigned char weight)override;

                unsigned char get_blend_fade()const override;
                void set_blend_fade(unsigned char fade)override;
            private:
                bool m_use_sub_display;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_CONTROLLERS_HPP
