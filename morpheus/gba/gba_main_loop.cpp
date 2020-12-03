//
// Created by bobby on 25/07/2020.
//

#include "gba_main_loop.hpp"

[[noreturn]] morpheus::core::Error morpheus::gba::GbaMainLoop::game_loop() {
    platform_init();

    while(true) {
        key_poll();

        std::vector<morpheus::core::InputEvent> down_events = to_input_events(key_hit(KEY_FULL),
                                                                              GBA_KEYPAD_BITS, GBA_KEYPAD_BITS_SIZE,
                                                                              morpheus::core::InputState::DOWN);
        std::vector<morpheus::core::InputEvent> held_events = to_input_events(key_is_down(KEY_FULL),
                                                                              GBA_KEYPAD_BITS, GBA_KEYPAD_BITS_SIZE,
                                                                    morpheus::core::InputState::HELD);
        std::vector<morpheus::core::InputEvent> up_events = to_input_events(key_released(KEY_FULL),
                                                                            GBA_KEYPAD_BITS, GBA_KEYPAD_BITS_SIZE,
                                                                            morpheus::core::InputState::UP);

        std::vector<core::InputEvent> input_events;


        input_events.insert(input_events.end(), down_events.begin(), down_events.end());
        input_events.insert(input_events.end(), held_events.begin(), held_events.end());
        input_events.insert(input_events.end(), up_events.begin(), up_events.end());

        for(core::InputEvent input_event : input_events){
            m_root->received_input(input_event);
        }

        m_root->draw(&m_obj_buffer, 0);

        oam_copy(oam_mem, m_obj_buffer, 128);

        VBlankIntrWait();
    }
    //return morpheus::core::OK;
}

morpheus::core::Error morpheus::gba::GbaMainLoop::platform_init() {
    irq_init(nullptr);
    irq_enable(eIrqIndex::II_VBLANK);

    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0;
    REG_IME = 1;

    tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

    oam_init(m_obj_buffer, 128);

    return morpheus::core::Error::OK;
}

morpheus::core::InputEvent
morpheus::core::MainLoop::to_input_event(uint32_t inputs,
                                         uint16_t keypad_bit,
                                         morpheus::core::InputState input_state) {
    morpheus::core::InputButton input_button = morpheus::core::InputButton::NONE;
    morpheus::core::InputEvent input_event{};

    if((inputs & keypad_bit) != 0) {
        switch (keypad_bit) {
            case KEY_A:
                input_button = morpheus::core::InputButton::A;
                break;
            case KEY_B:
                input_button = morpheus::core::InputButton::B;
                break;
            case KEY_SELECT:
                input_button = morpheus::core::InputButton::SELECT;
                break;
            case KEY_START:
                input_button = morpheus::core::InputButton::START;
                break;
            case KEY_RIGHT:
                input_button = morpheus::core::InputButton::DPADRIGHT;
                break;
            case KEY_LEFT:
                input_button = morpheus::core::InputButton::DPADLEFT;
                break;
            case KEY_UP:
                input_button = morpheus::core::InputButton::DPADUP;
                break;
            case KEY_DOWN:
                input_button = morpheus::core::InputButton::DPADDOWN;
                break;
            case KEY_R:
                input_button = morpheus::core::InputButton::R;
                break;
            case KEY_L:
                input_button = morpheus::core::InputButton::L;
                break;
            default:
                // already set to InputButton::NONE
                break;
        }
    }

    input_event.button = input_button;
    input_event.state = input_state;

    return input_event;
}
