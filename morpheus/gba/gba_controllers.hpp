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
            protected:
               void update_blending_registers()override;
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
