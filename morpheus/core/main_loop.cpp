//
// Created by bobby on 01/12/2020.
//

#include "main_loop.hpp"

morpheus::core::MainLoop::MainLoop(morpheus::core::gfx::BlendingController *blending_controller,
                                   morpheus::core::CommunicationChannel *communication_channel,
                                   morpheus::core::gfx::MosaicController *mosaic_controller,
                                   morpheus::core::NoCashDebugController *no_cash_debug_controller,
                                   SaveManager *save_manager, bool enable_fat) {
    m_blending_controller.reset(blending_controller);
    m_communication_channel.reset(communication_channel);
    m_mosaic_controller.reset(mosaic_controller);
    m_no_cash_debug_controller.reset(no_cash_debug_controller);
    m_save_manager.reset(save_manager);

    m_sprites.reserve(MAX_SPRITE_NUM);

    #ifdef GBA_FAT_SUPPORT_ENABLED
        if(enable_fat) {
            fatInitDefault();
        }
    #endif
}

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

int morpheus::core::MainLoop::get_random_number(int max, int min, int supplementary_seed, bool use_mt) {
    // based of libtonc's qran_range function as explained here:
    // https://www.coranac.com/tonc/text/gfx.htm

    if(supplementary_seed == 1) {
        supplementary_seed = m_supplementary_seed;
    }

    if(use_mt) {
        std::mt19937 mt_engine(m_past_random_number * supplementary_seed);

        m_past_random_number = static_cast<int>(mt_engine());
    } else {
        m_past_random_number = 1664525 * (m_past_random_number * supplementary_seed) + 1013904223;
    }

    return (((m_past_random_number >> 16) & 0x7FFF) * (max - min) >> 15) + min;
}
