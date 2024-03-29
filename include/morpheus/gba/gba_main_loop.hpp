//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_GBA_MAIN_LOOP_HPP
#define MORPHEUS_GBA_MAIN_LOOP_HPP

#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <maxmod.h>
#include <tonc.h>

#include <morpheus/core/main_loop.hpp>
#include <morpheus/core/gfx/sprite_base.hpp>
#include <morpheus/core/gfx/window.hpp>

#include <morpheus/gba/gba_controllers.hpp>
#include <morpheus/gba/gba_main_loop.hpp>
#include <morpheus/gba/serial_communication.hpp>
#include <morpheus/gba/gba_eeprom_save_manager.hpp>
#include <morpheus/gba/gba_flash_save_manager.hpp>
#include <morpheus/gba/gba_sram_save_manager.hpp>

namespace morpheus {
    namespace gba {
        static constexpr uint16_t GBA_KEYPAD_BITS[] = { KEY_A, KEY_B, KEY_SELECT, KEY_START, KEY_RIGHT, KEY_LEFT,
                                                        KEY_UP, KEY_DOWN, KEY_R, KEY_L };
        static constexpr int GBA_KEYPAD_BITS_SIZE = 10;
        static constexpr int GBA_MAX_SPRITES = 128;

        class GbaMainLoop : public core::MainLoop {
        public:
            const static int OBJ_ATTR_SIZE = sizeof(OBJ_ATTR) / 8; ///< Internally
                                                                   ///< used libgba
                                                                   ///< size of OAM
                                                                   ///< OBJ
                                                                   ///< (or sprite)
                                                                   ///< attributes.

            /// Constructs the (single) GbaMainLoop object, initializing the
            /// save type and (optionally) enabling libfat support.
            /// \param save_type The current save type this MainLoop
            /// (and this Morpheus game) will use.
            /// \param enable_fat Whether to enable libfat support or not
            explicit GbaMainLoop(core::GbaSaveType save_type, bool enable_fat = false);

            /// Destructs the GbaMainLoop object.
            virtual ~GbaMainLoop();

            void clear_obj_vram()override;

            void disable_affine()override;
            void disable_window(core::gfx::WindowType window_type)override;

            void enable_affine(core::gfx::AffineMode affine_mode)override;
            void enable_background(unsigned int background_num)override;
            void enable_window(core::gfx::WindowType window_type)override;

            [[noreturn]] core::Error game_loop()override;
        protected:
            core::Error platform_init()override;

            core::InputEvent to_input_event(uint32_t inputs, uint16_t keypad_bit,
                                            morpheus::core::InputState input_state)override;
        private:
            class DebugStream : std::ostringstream {
            public:
                DebugStream(GbaMainLoop *main_loop) {
                    m_main_loop = main_loop;
                }

                void refresh_and_print();
                std::streambuf *rdbuf_string_stream() {
                    return rdbuf();
                }
            private:
                GbaMainLoop *m_main_loop;
            };

            static core::SaveManager *select_appropriate_save_manager(core::GbaSaveType save_type);

            std::unique_ptr<DebugStream> m_debug_stream;
            std::vector<void *> m_obj_buffer;

            unsigned int m_backgrounds_to_enable = 0x0;
            unsigned int m_last_input_size = 0;
            bool m_platform_inited = false;
            bool m_using_tte = false;
            unsigned int m_windows_to_enable = 0x0;
        };

        /// \class morpheus::gba::GbaMainLoop
        /// The GBA implementation of morpheus::core::MainLoop, representing
        /// a GBA Morpheus game's main loop. There should only be a single
        /// GbaMainLoop over the GBA game's lifecycle, otherwise undefined
        /// behavior will occur. For more information about this class,
        /// consult the aforementioned parent class.
    }
}

#endif //MORPHEUS_GBA_MAIN_LOOP_HPP
