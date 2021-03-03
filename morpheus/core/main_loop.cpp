//
// Created by bobby on 01/12/2020.
//

#include "main_loop.hpp"


int morpheus::core::MainLoop::past_random_number = 0;
bool morpheus::core::MainLoop::mt_inited = false;
unsigned short morpheus::core::MainLoop::r256table[256];
unsigned char morpheus::core::MainLoop::r256index;

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

int morpheus::core::MainLoop::get_random_number(int max, int min, bool use_mt) {
    // based of libtonc's qran_range function as explained here:
    // https://www.coranac.com/tonc/text/gfx.htm

    if(use_mt) {
        std::mt19937 mt_engine(past_random_number);

        past_random_number = static_cast<int>(mt_engine());
    } else {
        past_random_number = 1664525 * past_random_number + 1013904223;
    }

    return (((past_random_number >> 16) & 0x7FFF) * (max - min) >> 15) + min;
}
