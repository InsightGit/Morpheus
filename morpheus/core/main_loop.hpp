//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_MAIN_LOOP_HPP
#define MORPHEUS_MAIN_LOOP_HPP

#include <vector>

#include <core/input_event.hpp>
#include <core/node.hpp>
#include <core/uncopyable.hpp>

namespace morpheus {
    namespace core {
        enum class Error {
            OK
        };

        class Node;

        class MainLoop : Uncopyable {
        public:
            void set_root(std::shared_ptr<Node> root) {
                m_root = root;
            }

            virtual void enable_background(unsigned int background_num) = 0;

            virtual Error game_loop() = 0;
        protected:
            std::vector<InputEvent> to_input_events(const uint32_t inputs, const uint16_t input_bits[],
                                                    int input_bits_size, const morpheus::core::InputState input_state);

            virtual InputEvent to_input_event(const uint32_t inputs, const uint16_t keypad_bit,
                                              const InputState input_state) = 0;

            virtual Error platform_init() = 0;

            std::shared_ptr<Node> m_root;
            //std::unique_ptr<PaletteManager> m_palette_manager(new );
        };
    }
}

#endif //MORPHEUS_MAIN_LOOP_HPP
