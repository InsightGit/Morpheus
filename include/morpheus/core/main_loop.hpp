//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_MAIN_LOOP_HPP
#define MORPHEUS_MAIN_LOOP_HPP

#include <algorithm>
#include <random>
#include <vector>

#ifdef GBA_FAT_SUPPORT_ENABLED
    #include <fat.h>
#endif

#include "gfx/sprite_base.hpp"
#include "gfx/window.hpp"
#include "communication_channel.hpp"
#include "controllers.hpp"
#include "control_reciever.hpp"
#include "input_event.hpp"
#include "save_manager.hpp"
#include "uncopyable.hpp"

namespace morpheus {
    namespace core {
        enum class Error {
            OK
        };

        namespace gfx {
            enum class AffineMode {
                MIXED_AFFINE, ///< Mixed Affine (Mode 1 on the GBA/NDS)
                              ///< This displays backgrounds 0 and 1 as regular
                              ///< backgrounds, background 2 as affine, and
                              ///< disables background 3 on the GBA. On the DS,
                              ///< no backgrounds are disabled and backgrounds
                              ///< 0-2 are displayed as regular backgrounds and
                              ///< background 3 is displayed as an affine one.
                FULL_AFFINE ///< Full Affine (Mode 2 on the GBA/NDS)
                            ///< On the GBA, this disables backgrounds 0 and 1
                            ///< and displays backgrounds 2 and 3 as affine
                            ///< backgrounds. On the DS, no backgrounds are
                            ///< disabled, backgrounds 0 and 1 are displayed as
                            ///< regular backgrounds, and backgrounds 2 and 3
                            ///< are displayed as affine
            };

            /// \enum morpheus::core::gfx::AffineMode
            /// Enum class representing the different affine modes supported
            /// on the GBA and the DS.

            enum class WindowType;
            class SpriteBase;
        }

        enum class GbaSaveType {
            EEPROM_512B, ///< 512 byte EEPROM (currently WIP)
            EEPROM_8KB, ///< 8 kilobyte EEPROM (currently WIP)
            SRAM_32KB, ///< 32 kilobyte SRAM/FRAM
            FLASH_AUTO_DETECT, ///< 64 or 128 kilobyte FLASH
            FLASH_64KB, ///< 64 kilobyte FLASH
            FLASH_128KB ///< 128 kilobyte FLASH
        };

        /// \enum morpheus::core::gfx::GbaSaveType
        /// All the save types supported inside Morpheus for the GBA.

        class ControlReciever;

        const int MAX_SPRITE_NUM = 128; ///< The maximum number of sprites
                                        ///< that can be displayed at once
        const int VBLANK_CYCLE_TIME = 60; ///< The number of VBlanks per one
                                          ///< second

        class MainLoop : Uncopyable {
        public:
            /// Constructs a main loop for the game to run within.
            /// NOTE: this class is abstract and shouldn't be used in isolation.
            /// Platform-specific classes like morpheus::nds::NdsMainLoop and
            /// morpheus::gba::GbaMainLoop implement and construct this class.
            /// \param blending_controller Platform-specific blending controller
            /// \param communication_channel Platform-specific communication channel
            /// \param mosaic_controller Platform-specific mosaic controller
            /// \param no_cash_debug_controller Platform-specific debug controller
            /// \param save_manager Platform-specific save manager
            /// \param enable_fat Whether to enable libfat or not.
            MainLoop(gfx::BlendingController *blending_controller, CommunicationChannel *communication_channel,
                     gfx::MosaicController *mosaic_controller, NoCashDebugController *no_cash_debug_controller,
                     SaveManager *save_manager, bool enable_fat);

            /// Destructs the Main loop object.
            virtual ~MainLoop() = default;

            /// Adds a ControlReciever to this MainLoop, calling its ControlReciever::input and
            /// ControlReciever::update functions within MainLoop::game_loop
            /// \param control_reciever the ControlReciever to add
            void add_control_reciever(std::shared_ptr<ControlReciever> control_reciever) {
                m_control_recievers.push_back(control_reciever);
            }

            /// Adds a SpriteBase to this MainLoop if we haven't exceeded
            /// MainLoop::MAX_SPRITE_NUM (usually 128) and the sprite
            /// wasn't already added. If added, the main game loop will call its
            /// SpriteBase::input, SpriteBase::update, and SpriteBase::draw
            /// functions within MainLoop::game_loop
            /// \param sprite the SpriteBase to add
            /// \return Whether the sprite was successfully added
            bool add_sprite(std::shared_ptr<gfx::SpriteBase> sprite) {
                if(m_sprites.size() < MAX_SPRITE_NUM &&
                   std::find(m_sprites.begin(), m_sprites.end(), sprite) == m_sprites.end()) {
                    m_sprites.push_back(sprite);

                    return true;
                } else {
                    return false;
                }
            }

            /// Returns the current platform-specific gfx::BlendingController
            /// for the hardware blending effect.
            /// \return The platform-specific gfx::BlendingController in use
            gfx::BlendingController *get_blending_controller() const {
                return m_blending_controller.get();
            }

            /// Returns the current platform-specific gfx::MosaicController
            /// for the hardware mosaic effect.
            /// \return The platform-specific gfx::MosaicController
            gfx::MosaicController *get_mosaic_controller() const {
                return m_mosaic_controller.get();
            }

            /// Returns the current platform-specific NoCashDebugController
            /// for debugging.
            /// \return The platform-specific NoCashDebugController in use
            NoCashDebugController *get_no_cash_debug_controller() const {
                return m_no_cash_debug_controller.get();
            }

            /// Returns the current platform-specific CommunicationChannel for
            /// multiplayer communication (currently only implemented on the GBA
            /// for the link cable).
            /// \return The platform-specific NoCashDebugController in use
            CommunicationChannel *get_default_communication_channel() const {
                return m_communication_channel.get();
            }

            /// Returns the current platform-specific SaveManager for save and
            /// non-volatile memory management.
            /// \return The platform-specific SaveManager in use
            SaveManager *get_save_manager() const {
                return m_save_manager.get();
            }

            /// Returns whether the GBA or the DS is currently on graphical
            /// mode that supports affine sprites and backgrounds
            /// (Modes 1 and 2).
            /// \return Whether the current graphical mode supports affine
            /// sprites and backgrounds
            bool is_affine() const {
                return m_affine;
            }

            /// Returns whether the GBA or the DS is in a partial affine mode
            /// (Mode 1) or a full affine mode (Mode 2).
            /// \return The current affine mode used
            gfx::AffineMode get_affine_mode() const {
                return m_affine_mode;
            }

            /// Removes ControlReciever from this MainLoop if it was added.
            /// If removed, its ControlReciever::input and
            /// ControlReciever::update functions will no longer be called
            /// within MainLoop::game_loop.
            /// \param control_reciever the ControlReciever to be removed
            void remove_control_reciever(ControlReciever *control_reciever) {
                m_control_recievers.erase(std::remove_if(m_control_recievers.begin(), m_control_recievers.end(),
                               [this, control_reciever](std::shared_ptr<ControlReciever> other) {
                    return other.get() == control_reciever;
                }), m_control_recievers.end());
            }

            void remove_control_reciever(std::shared_ptr<ControlReciever> control_reciever) {
                remove_control_reciever(control_reciever.get());
            }

            /// Removes a gfx::SpriteBase from this MainLoop if the gfx::SpriteBase has
            /// been added to this MainLoop. If added, the main game loop will
            /// call its gfx::SpriteBase::input, gfx::SpriteBase::update,
            /// and gfx::SpriteBase::draw functions within gfx::MainLoop::game_loop
            /// \param sprite the SpriteBase to remove
            void remove_sprite(gfx::SpriteBase *sprite) {
                m_sprites.erase(std::remove_if(m_sprites.begin(), m_sprites.end(),
                               [this, sprite](std::shared_ptr<gfx::SpriteBase> other) {
                   bool return_value = other.get() == sprite;

                   if(return_value) {
                       sprite->hide();
                   }

                   return return_value;
               }), m_sprites.end());
            }

            void remove_sprite(std::shared_ptr<gfx::SpriteBase> sprite) {
                remove_sprite(sprite.get());
            }

            /// Generates a random number based on a supplementary seed and
            /// the previous random number generated. Either uses a C++11
            /// Mersenne Twister engine or a linear congruential generator
            /// engine.
            /// \param max The maximum random number that could be generated
            /// \param min The minimum random number that could be generated
            /// \param supplementary_seed The supplementary seed that will be
            /// combined with the last generated random number
            /// \param use_mt Whether to use the C++11 Mersenne Twister engine
            /// (true) or a linear congruential generator engine (false)
            /// \return The random number generated
            int get_random_number(int max, int min, int supplementary_seed = 1, bool use_mt = true);

            /// Clears the Object (or Sprite) VRAM. Not currently implemented.
            virtual void clear_obj_vram() = 0;

            /// Disables support for affine backgrounds and sprites.
            virtual void disable_affine() = 0;

            /// Disables a certain Window based on the gfx::WindowType.
            /// \param window_type The gfx::WindowType of the Window to disable
            virtual void disable_window(gfx::WindowType window_type) = 0;

            /// Enables support for affine backgrounds and sprites.
            /// Can either use AffineMode::MIXED_AFFINE for activating Mode 1
            /// (Background 3 (DS) or 2 (GBA) is affine, and Background 3 on the
            /// GBA is disabled) or AffineMode::FULL_AFFINE for activating Mode
            /// 2 (Backgrounds 2 and 3 are affine, and Backgrounds 0 and 1 on
            /// the GBA are disabled).
            /// \param affine_mode The gfx::AffineMode to enable
            virtual void enable_affine(gfx::AffineMode affine_mode) = 0;

            /// Enables a certain background. Unless on the GBA with an affine
            /// mode active, there should be 3 backgrounds numbered 0 to 3 that
            /// can be used.
            /// \param background_num The background number to enable
            virtual void enable_background(unsigned int background_num) = 0;

            /// Enables a certain gfx::Window based on the gfx::WindowType, which can include
            /// window 0, window 1, the object window, or the out window.
            /// \param window_type The gfx::WindowType of the Window to enable
            virtual void enable_window(gfx::WindowType window_type) = 0;

            /// Starts the game loop. This function should be called after all
            /// initial game elements are setup and the game is ready to be
            /// played.
            /// \return The Error (if any) in setting up the game loop.
            /// Currently nonexistent (always returns Error::OK) for the GBA
            /// or the DS.
            virtual Error game_loop() = 0;
        protected:
            /// Sets whether affine is enabled on this MainLoop or not.
            /// \param affine The new affine state on this MainLoop.
            void set_affine(const bool affine) {
                m_affine = affine;
            }

            /// Sets what affine mode is used on this MainLoop while affine
            /// support is enabled.
            /// \param affine The new AffineMode to set on this MainLoop.
            void set_affine_mode(const gfx::AffineMode affine_mode) {
                m_affine_mode = affine_mode;
            }

            /// Sets the SaveManager being used by this MainLoop
            /// \param save_manager The new SaveManager for this MainLoop to use
            void set_save_manager(SaveManager *save_manager) {
                m_save_manager = std::unique_ptr<SaveManager>(save_manager);
            }

            /// Sets a supplementary seed by a specific implementation of this
            /// MainLoop that this abstract MainLoop can use to help randomize
            /// MainLoop::get_random_number().
            /// \param supplementary_seed The new supplemetary seed to be set on
            /// this MainLoop
            void set_supplementary_seed(const int supplementary_seed) {
                m_supplementary_seed = supplementary_seed;
            }

            /// Converts a generated input integer using platform-specific
            /// integer bits into an InputEvent std::vector for input event
            /// handling.
            /// \param inputs The generated input integer to use
            /// \param input_bits The buffer of the platform-specific integer
            /// bits
            /// \param input_bits_size The size of the the integer bit buffer
            /// \param input_state The InputState of the generated input integer
            /// \return The InputEvent std::vector produced
            std::vector<InputEvent> to_input_events(const uint32_t inputs, const uint16_t input_bits[],
                                                    int input_bits_size, const morpheus::core::InputState input_state);

            /// Pure virtual function which converts a specific keypad bit from
            /// the generated input integer to a InputEvent for input event
            /// handling in a platform specific way.
            /// \param inputs The generated input integer to use
            /// \param keypad_bit The specific keypad bit to use
            /// \param input_state The InputState of the generated InputEvent
            /// \return The InputEvent produced
            virtual InputEvent to_input_event(const uint32_t inputs, const uint16_t keypad_bit,
                                              const InputState input_state) = 0;

            /// Initializes some platform-specific functions right before the
            /// first iteration of MainLoop::game_loop() is completed.
            virtual Error platform_init() = 0;

            unsigned char m_cycle_time; ///< The current VBlank number that this
                                        ///< MainLoop is on.

            std::vector<std::shared_ptr<ControlReciever>> m_control_recievers; ///< The
                                                                               ///< ControlRecievers
                                                                               ///< added
                                                                               ///< to
                                                                               ///< this
                                                                               ///< MainLoop.
            std::vector<std::shared_ptr<gfx::SpriteBase>> m_sprites; ///< The
                                                                     ///< Sprites
                                                                     ///< added
                                                                     ///< to
                                                                     ///< this
                                                                     ///< MainLoop.
        private:
            std::unique_ptr<gfx::BlendingController> m_blending_controller;
            std::unique_ptr<CommunicationChannel> m_communication_channel;
            std::unique_ptr<gfx::MosaicController> m_mosaic_controller;
            std::unique_ptr<NoCashDebugController> m_no_cash_debug_controller;
            std::unique_ptr<SaveManager> m_save_manager;

            bool m_affine = false;
            gfx::AffineMode m_affine_mode;
            bool m_mt_inited;
            unsigned short m_r256table[256];
            unsigned char m_r256index;
            int m_past_random_number;
            unsigned int m_sprite_count = 0;
            int m_supplementary_seed;
        };


        /// \class morpheus::core::MainLoop
        /// Abstract class whose implementors control the main game loop and the
        /// main display control register. This class should be initialized and
        /// MainLoop::game_loop should be called within every Morpheus game as so:
        /// \code
        ///     std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
        ///
        ///     // Do initialization of the sprites, and backgrounds to be initially displayed
        ///
        ///     main_loop->game_loop();
        /// \endcode
    }
}

#endif //MORPHEUS_MAIN_LOOP_HPP
