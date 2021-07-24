//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_CONTROLLERS_HPP
#define MORPHEUS_GBA_TEST_GBA_CONTROLLERS_HPP

#include <algorithm>
#include <string>

#include <tonc.h>

#include <morpheus/core/controllers.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class GbaBlendingController : public morpheus::core::gfx::BlendingController {
            public:
                /// Destructs the GbaBlendingController object.
                ~GbaBlendingController() override = default;
            protected:
               void update_blending_registers()override;
            };

            /// \class morpheus::gba::gfx::GbaBlendingController
            /// The Game Boy Advance (GBA) implementation of
            /// morpheus::core::gfx::BlendingController. For more details about
            /// this class, consult the documentation of the aforementioned
            /// parent class.

            class GbaMosaicController : public morpheus::core::gfx::MosaicController {
            public:
                /// Destructs the GbaMosaicController object.
                ~GbaMosaicController() override = default;
            protected:
                void update_mosaic_register()override;
            };

            /// \class morpheus::gba::gfx::GbaMosaicController
            /// The Game Boy Advance (GBA) implementation of
            /// morpheus::core::gfx::MosaicController. For more details about
            /// this class, consult the documentation of the aforementioned
            /// parent class.
        }

        class GbaNoCashDebugController : public core::NoCashDebugController {
        public:
            void send_to_debug_window(std::string message) override {
                nocash_puts(message.c_str());
            }
        };

        /// \class morpheus::gba::GbaNocashDebugController
        /// Game Boy Advance (GBA) implementation of
        /// morpheus::core::NocashDebugController. For more details about
        /// this class, consult the documentation of the aforementioned parent
        /// class.
    }
}


#endif //MORPHEUS_GBA_TEST_GBA_CONTROLLERS_HPP
