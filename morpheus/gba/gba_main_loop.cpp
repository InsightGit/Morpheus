//
// Created by bobby on 25/07/2020.
//

//#include <tonc_libgba.h>
#include "gba_main_loop.hpp"

[[noreturn]] morpheus::core::Error morpheus::gba::GbaMainLoop::game_loop() {
    platform_init();

    while(true) {
        key_poll();

        std::list<morpheus::core::InputEvent> down_events = to_input_events(key_hit(KEY_FULL),
                                                                            morpheus::core::InputState::DOWN);
        std::list<morpheus::core::InputEvent> held_events = to_input_events(key_is_down(KEY_FULL),
                                                                            morpheus::core::InputState::HELD);
        std::list<morpheus::core::InputEvent> up_events = to_input_events(key_released(KEY_FULL),
                                                                          morpheus::core::InputState::UP);

        std::list<core::InputEvent> input_events;


        input_events.splice(input_events.end(), down_events);
        input_events.splice(input_events.end(), held_events);
        input_events.splice(input_events.end(), up_events);

        for(core::InputEvent input_event : input_events){
            m_root->received_input(input_event);
        }

        // oh god this is S K E T C H
        m_obj_buffer = reinterpret_cast<OBJ_ATTR **>(m_root->draw(reinterpret_cast<void **>(m_obj_buffer), 0));

        oam_copy(oam_mem, *m_obj_buffer, 128);

        VBlankIntrWait();
    }
    //return morpheus::core::OK;
}

morpheus::core::Error morpheus::gba::GbaMainLoop::platform_init() {
    irq_init(nullptr);
    irq_enable(eIrqIndex::II_VBLANK);

    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D;
    REG_IME = 1;

    oam_init(*m_obj_buffer, 128);

    return morpheus::core::Error::OK;
}

morpheus::core::InputEvent
    morpheus::gba::GbaMainLoop::to_input_event(uint16_t libgba_inputs,
                                               uint16_t keypad_bit,
                                               morpheus::core::InputState input_state) {
    morpheus::core::InputButton input_button = morpheus::core::InputButton::NONE;
    morpheus::core::InputEvent input_event{};

    if((libgba_inputs & keypad_bit) != 0) {
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

std::list<morpheus::core::InputEvent>
    morpheus::gba::GbaMainLoop::to_input_events(u16 libgba_inputs,
                                                morpheus::core::InputState input_state) {
    std::list<morpheus::core::InputEvent> input_events;

    for(uint16_t gba_keypad_bit : GBA_KEYPAD_BITS) {
        morpheus::core::InputEvent input_event = to_input_event(libgba_inputs, gba_keypad_bit, input_state);

        if(input_event.button != morpheus::core::InputButton::NONE) {
            input_events.push_back(input_event);
        }
    }

    return input_events;
}

