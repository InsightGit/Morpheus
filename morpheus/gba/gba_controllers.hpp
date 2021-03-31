//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_CONTROLLERS_HPP
#define MORPHEUS_GBA_TEST_GBA_CONTROLLERS_HPP

#include <algorithm>
#include <string>

#include <tonc.h>

#include <core/controllers.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class GbaBlendingController : public morpheus::core::gfx::BlendingController {
            public:
                virtual ~GbaBlendingController() {}

                void disable_backdrop_blending()override;
                void enable_backdrop_blending(bool bottom)override;

                void disable_background_blending(unsigned int background)override;
                void enable_background_blending(bool bottom, unsigned int background)override;

                void disable_object_blending()override;
                void enable_object_blending(bool bottom)override;

                core::gfx::BlendingMode get_blending_mode()override;
                void set_blending_mode(core::gfx::BlendingMode blending_mode)override;

                unsigned char get_blend_weight(bool bottom)const override;
                void set_blend_weight(bool bottom, unsigned char weight)override;

                unsigned char get_blend_fade()const override;
                void set_blend_fade(unsigned char fade)override;
            };

            class GbaMosaicController : public morpheus::core::gfx::MosaicController {
            public:
                virtual ~GbaMosaicController() {}
            protected:
                void update_mosaic_register()override;
            };
        }

        class NdsNoCashDebugController : public core::NoCashDebugController {
        public:
            void send_to_debug_window(std::string message) override {
                nocash_puts(message.c_str());
            }
        };
    }
}


#endif //MORPHEUS_GBA_TEST_GBA_CONTROLLERS_HPP
