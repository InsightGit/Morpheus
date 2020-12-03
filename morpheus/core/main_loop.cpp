//
// Created by bobby on 01/12/2020.
//

#include "main_loop.hpp"


std::vector<morpheus::core::InputEvent>
morpheus::core::MainLoop::to_input_events(const uint32_t inputs, const uint16_t input_bits[],
                                          int input_bits_size, const morpheus::core::InputState input_state) {
    std::vector<morpheus::core::InputEvent> input_events;

    for(int i = 0; input_bits_size > i; ++i) {
        morpheus::core::InputEvent input_event = to_input_event(inputs, input_bits[i], input_state);

        if(input_event.button != morpheus::core::InputButton::NONE) {
            input_events.push_back(input_event);
        }
    }

    return input_events;
}