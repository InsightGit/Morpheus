//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_MAIN_LOOP_HPP
#define MORPHEUS_MAIN_LOOP_HPP

#include <random>
#include <vector>

#include <core/gfx/window.hpp>
#include <core/input_event.hpp>
#include <core/node.hpp>
#include <core/uncopyable.hpp>

namespace morpheus {
    namespace core {
        enum class Error {
            OK
        };

        namespace gfx {
            enum class WindowType;
        }

        class Node;

        const int VBLANK_CYCLE_TIME = 60;

        class MainLoop : Uncopyable {
        public:
            virtual ~MainLoop() = default;

            void set_root(std::shared_ptr<Node> root) {
                m_root = root;
            }

            int get_random_number(int max, int min, int supplementary_seed = 1, bool use_mt = true);

            virtual void clear_obj_vram() = 0;

            virtual void disable_window(gfx::WindowType window_type) = 0;

            virtual void enable_background(unsigned int background_num) = 0;
            virtual void enable_window(gfx::WindowType window_type) = 0;

            virtual void send_to_debug_window(std::string string) = 0;

            virtual Error game_loop() = 0;
        protected:
            void set_supplementary_seed(int supplementary_seed) {
                m_supplementary_seed = supplementary_seed;
            }

            std::vector<InputEvent> to_input_events(const uint32_t inputs, const uint16_t input_bits[],
                                                    int input_bits_size, const morpheus::core::InputState input_state);

            virtual InputEvent to_input_event(const uint32_t inputs, const uint16_t keypad_bit,
                                              const InputState input_state) = 0;

            virtual Error platform_init() = 0;

            unsigned char m_cycle_time;
            std::shared_ptr<Node> m_root;
            //std::unique_ptr<PaletteManager> m_palette_manager(new );
        private:
            bool m_mt_inited;
            unsigned short m_r256table[256];
            unsigned char m_r256index;
            int m_past_random_number;
            int m_supplementary_seed;
        };
    }
}

#endif //MORPHEUS_MAIN_LOOP_HPP
