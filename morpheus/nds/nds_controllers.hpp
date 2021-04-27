//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_NDS_TEST_NDS_CONTROLLERS_HPP
#define MORPHEUS_NDS_TEST_NDS_CONTROLLERS_HPP

#include <nds.h>

#include <core/core.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class NdsBlendingController : public morpheus::core::gfx::BlendingController {
            public:
                explicit NdsBlendingController(bool use_sub_display) {
                    m_use_sub_display = use_sub_display;
                }

                virtual ~NdsBlendingController() = default;
            protected:
                void update_blending_registers()override;
            private:
                bool m_use_sub_display;
            };

            class NdsMosaicController : public morpheus::core::gfx::MosaicController {
            public:
                explicit NdsMosaicController(bool use_sub_display) {
                    m_use_sub_display = use_sub_display;
                }

                virtual ~NdsMosaicController() = default;
            protected:
                void update_mosaic_register()override;
            private:
                bool m_use_sub_display;
            };
        }

        class NdsNoCashDebugController : public core::NoCashDebugController {
        public:
            void send_to_debug_window(std::string message) override {
                nocashMessage(message.c_str());
            }
        };
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_CONTROLLERS_HPP
