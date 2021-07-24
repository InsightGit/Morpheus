//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_NDS_TEST_NDS_CONTROLLERS_HPP
#define MORPHEUS_NDS_TEST_NDS_CONTROLLERS_HPP

#include <nds.h>

#include <morpheus/core/core.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            class NdsBlendingController : public morpheus::core::gfx::BlendingController {
            public:
                /// Constructs a blending controller for the DS for either the
                /// bottom sub display or the top main display. Note: this class
                /// should not be manually constructed and should instead be
                /// retrieved through the MainLoop. See parent class
                /// morpheus::core::gfx::BlendingController documentation for
                /// more details.
                /// \param use_sub_display Whether this blending controller
                /// will control the bottom sub display (true) or the top
                /// main display (false).
                explicit NdsBlendingController(bool use_sub_display) {
                    m_use_sub_display = use_sub_display;
                }

                /// Destroys the NdsBlendingController object.
                virtual ~NdsBlendingController() = default;
            protected:
                void update_blending_registers()override;
            private:
                bool m_use_sub_display;
            };

            /// \class morpheus::nds::gfx::NdsBlendingController
            /// The Nintendo DS (NDS) implementation of
            /// morpheus::core::gfx::BlendingController. For more details about
            /// this class, consult the documentation of the aforementioned
            /// parent class.

            class NdsMosaicController : public morpheus::core::gfx::MosaicController {
            public:
                /// Constructs a mosaic controller for the DS for either the
                /// bottom sub display or the top main display. Note: this class
                /// should not be manually constructed and should instead be
                /// retrieved through the MainLoop. See parent class
                /// morpheus::core::gfx::MosaicController documentation for
                /// more details.
                /// \param use_sub_display Whether this mosaic controller
                /// will control the bottom sub display (true) or the top
                /// main display (false).
                explicit NdsMosaicController(bool use_sub_display) {
                    m_use_sub_display = use_sub_display;
                }

                /// Destroys the NdsMosaicController object.
                virtual ~NdsMosaicController() = default;
            protected:
                void update_mosaic_register()override;
            private:
                bool m_use_sub_display;
            };

            /// \class morpheus::nds::gfx::NdsMosaicController
            /// The Nintendo DS (NDS) implementation of
            /// morpheus::core::gfx::MosaicController. For more details about
            /// this class, consult the documentation of the aforementioned
            /// parent class.
        }

        class NdsNoCashDebugController : public core::NoCashDebugController {
        public:
            void send_to_debug_window(std::string message) override {
                nocashMessage(message.c_str());
            }
        };

        /// \class morpheus::nds::NdsNoCashDebugController
        /// The Nintendo DS (NDS) implementation of
        /// morpheus::core::gfx::NoCashDebugController. For more details about
        /// this class, consult the documentation of the aforementioned
        /// parent class.
    }
}

#endif //MORPHEUS_GBA_TEST_NDS_CONTROLLERS_HPP
