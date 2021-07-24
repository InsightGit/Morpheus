//
// Created by bobby on 25/11/2020.
//

#ifndef MORPHEUS_NDS_MAIN_LOOP_HPP
#define MORPHEUS_NDS_MAIN_LOOP_HPP

#include <cstdio>

#include <nds.h>

#include <morpheus/core/main_loop.hpp>
#include <morpheus/core/gfx/window.hpp>

#include <morpheus/nds/nds_controllers.hpp>
#include <morpheus/nds/ds_flashcard_save_manager.hpp>


namespace morpheus {
    namespace nds {
        static constexpr uint16_t NDS_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L,
                                                        /* DS exclusive keys */
                                                        KEY_X, KEY_Y, KEY_TOUCH, KEY_LID };
        static constexpr int NDS_KEYPAD_BITS_SIZE = 14;

        class NdsMainLoop : public core::MainLoop {
        public:
            /// Constructs the (single) NdsMainLoop object.
            explicit NdsMainLoop();

            static void reset_to_debug_print_console() {
                if(debug_print_console != nullptr) {
                    consoleSelect(debug_print_console);
                }
            }

            /// \return The BlendingController of this NdsMainLoop for the
            /// bottom sub display. (The regular get_blending_controller()
            /// returns the BlendingController for the top main display).
            core::gfx::BlendingController *get_sub_blending_controller() const {
                return m_sub_blending_controller.get();
            }

            /// \return The MosaicController of this NdsMainLoop for the
            /// bottom sub display. (The regular get_blending_controller()
            /// returns the MosaicController for the top main display).
            core::gfx::MosaicController *get_sub_mosaic_controller() const {
                return m_sub_mosaic_controller.get();
            }

            void clear_obj_vram()override;

            void disable_window(core::gfx::WindowType window_type)override;

            void disable_affine() override {
                disable_affine(false);
            }

            void enable_affine(core::gfx::AffineMode affine_mode) override {
                enable_affine(affine_mode, false);
            }

            /// Disables affine support on either the bottom sub display or the
            /// top main display.
            /// \param use_sub_display Whether to disable the support on the
            /// bottom sub display (true) or the top main display (false)
            void disable_affine(bool use_sub_display);

            /// Enables affine support on either the bottom sub display or the
            /// top main display.
            /// \param affine_mode The AffineMode to enable
            /// \param use_sub_display Whether to disable the support on the
            /// bottom sub display (true) or the top main display (false)
            void enable_affine(core::gfx::AffineMode affine_mode, bool use_sub_display);

            void enable_background(unsigned int background_reference_num)override;
            void enable_window(core::gfx::WindowType window_type)override;

            [[noreturn]] core::Error game_loop()override;
        protected:
            core::Error platform_init()override;

            core::InputEvent to_input_event(uint32_t inputs, uint16_t keypad_bit,
                                            morpheus::core::InputState input_state)override;
        private:
            static core::SaveManager *select_appropriate_save_manager();

            void setup_debug_console(bool use_main_display);

            static PrintConsole *debug_print_console;

            unsigned int m_last_input_size = 0;

            std::unique_ptr<core::SaveManager> m_save_manager;
            std::unique_ptr<gfx::NdsBlendingController> m_sub_blending_controller;
            std::unique_ptr<gfx::NdsMosaicController> m_sub_mosaic_controller;
        };

        /// \class morpheus::nds::NdsMainLoop
        /// The DS implementation of morpheus::core::MainLoop, representing
        /// a DS Morpheus game's main loop. There should only be a single
        /// NdsMainLoop over the DS game's lifecycle, otherwise undefined
        /// behavior will occur. For more information about this class,
        /// consult the aforementioned parent class.
    }
}

#endif //MORPHEUS_NDS_MAIN_LOOP_HPP
