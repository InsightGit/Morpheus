//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_MAIN_LOOP_HPP
#define MORPHEUS_MAIN_LOOP_HPP

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
            void set_root(std::shared_ptr<Node> root) {
                m_root = root;
            }

            static int get_random_number(int max, int min);

            virtual void disable_window(gfx::WindowType window_type) = 0;

            virtual void enable_background(unsigned int background_num) = 0;
            virtual void enable_window(gfx::WindowType window_type) = 0;

            virtual Error game_loop() = 0;
        protected:
            std::vector<InputEvent> to_input_events(const uint32_t inputs, const uint16_t input_bits[],
                                                    int input_bits_size, const morpheus::core::InputState input_state);

            virtual InputEvent to_input_event(const uint32_t inputs, const uint16_t keypad_bit,
                                              const InputState input_state) = 0;

            virtual Error platform_init() = 0;

            unsigned char m_cycle_time;
            std::shared_ptr<Node> m_root;
            //std::unique_ptr<PaletteManager> m_palette_manager(new );
        private:
            static bool mt_inited;
            static unsigned short r256table[256];
            static unsigned char r256index;
            static int past_random_number;
        };
    }
}

#endif //MORPHEUS_MAIN_LOOP_HPP
