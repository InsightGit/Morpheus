//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_MAIN_LOOP_HPP
#define MORPHEUS_MAIN_LOOP_HPP

#include <algorithm>
#include <random>
#include <vector>

#include <core/gfx/sprite_base.hpp>
#include <core/gfx/window.hpp>
#include <core/communication_channel.hpp>
#include <core/controllers.hpp>
#include <core/control_reciever.hpp>
#include <core/input_event.hpp>
#include <core/save_manager.hpp>
#include <core/uncopyable.hpp>

namespace morpheus {
    namespace core {
        enum class Error {
            OK
        };

        namespace gfx {
            enum class AffineMode {
                MIXED_AFFINE,
                FULL_AFFINE
            };

            enum class WindowType;
            class SpriteBase;
        }

        enum class GbaSaveType {
            EEPROM_8KB,
            EEPROM_512B,
            SRAM_32KB,
            FLASH_AUTO_DETECT,
            FLASH_64KB,
            FLASH_128KB
        };

        class ControlReciever;

        const int MAX_SPRITE_NUM = 128;
        const int VBLANK_CYCLE_TIME = 60;

        class MainLoop : Uncopyable {
        public:
            MainLoop(gfx::BlendingController *blending_controller, CommunicationChannel *communication_channel,
                     gfx::MosaicController *mosaic_controller, NoCashDebugController *no_cash_debug_controller,
                     SaveManager *save_manager);

            virtual ~MainLoop() = default;

            void add_control_reciever(std::shared_ptr<ControlReciever> control_reciever) {
                m_control_recievers.push_back(control_reciever);
            }

            bool add_sprite(std::shared_ptr<gfx::SpriteBase> sprite) {
                if(m_sprites.size() < MAX_SPRITE_NUM &&
                   std::find(m_sprites.begin(), m_sprites.end(), sprite) == m_sprites.end()) {
                    m_sprites.push_back(sprite);

                    return true;
                } else {
                    return false;
                }
            }

            gfx::BlendingController *get_blending_controller() const {
                return m_blending_controller.get();
            }

            gfx::MosaicController *get_mosaic_controller() const {
                return m_mosaic_controller.get();
            }

            NoCashDebugController *get_no_cash_debug_controller() const {
                return m_no_cash_debug_controller.get();
            }

            CommunicationChannel *get_default_communication_channel() const {
                return m_communication_channel.get();
            }

            SaveManager *get_save_manager() const {
                return m_save_manager.get();
            }

            bool is_affine() const {
                return m_affine;
            }

            gfx::AffineMode get_affine_mode() const {
                return m_affine_mode;
            }

            void remove_control_reciever(ControlReciever *control_reciever) {
                std::remove_if(m_control_recievers.begin(), m_control_recievers.end(),
                               [this, control_reciever](std::shared_ptr<ControlReciever> other) {
                    return other.get() == control_reciever;
                });
            }

            void remove_control_reciever(std::shared_ptr<ControlReciever> control_reciever) {
                remove_control_reciever(control_reciever.get());
            }

            void remove_sprite(gfx::SpriteBase *sprite) {
                std::remove_if(m_sprites.begin(), m_sprites.end(),
                               [this, sprite](std::shared_ptr<gfx::SpriteBase> other) {
                   bool return_value = other.get() == sprite;

                   if(return_value) {
                       sprite->hide();
                   }

                   return return_value;
               });
            }

            void remove_sprite(std::shared_ptr<gfx::SpriteBase> sprite) {
                remove_sprite(sprite.get());
            }

            int get_random_number(int max, int min, int supplementary_seed = 1, bool use_mt = true);

            virtual void clear_obj_vram() = 0;

            virtual void disable_affine() = 0;
            virtual void disable_window(gfx::WindowType window_type) = 0;

            virtual void enable_affine(gfx::AffineMode affine_mode) = 0;
            virtual void enable_background(unsigned int background_num) = 0;
            virtual void enable_window(gfx::WindowType window_type) = 0;

            virtual Error game_loop() = 0;
        protected:
            void set_affine(const bool affine) {
                m_affine = affine;
            }

            void set_affine_mode(const gfx::AffineMode affine_mode) {
                m_affine_mode = affine_mode;
            }

            void set_save_manager(SaveManager *save_manager) {
                m_save_manager = std::unique_ptr<SaveManager>(save_manager);
            }

            void set_supplementary_seed(const int supplementary_seed) {
                m_supplementary_seed = supplementary_seed;
            }

            std::vector<InputEvent> to_input_events(const uint32_t inputs, const uint16_t input_bits[],
                                                    int input_bits_size, const morpheus::core::InputState input_state);

            virtual InputEvent to_input_event(const uint32_t inputs, const uint16_t keypad_bit,
                                              const InputState input_state) = 0;

            virtual Error platform_init() = 0;

            unsigned char m_cycle_time;

            std::vector<std::shared_ptr<ControlReciever>> m_control_recievers;
            std::vector<std::shared_ptr<gfx::SpriteBase>> m_sprites;
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
    }
}

#endif //MORPHEUS_MAIN_LOOP_HPP
