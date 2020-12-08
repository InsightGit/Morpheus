//
// Created by bobby on 25/11/2020.
//

#include "nds_main_loop.hpp"

morpheus::core::Error morpheus::nds::NdsMainLoop::game_loop() {
    platform_init();

    while(true) {
        scanKeys();

        std::vector<morpheus::core::InputEvent> down_events = to_input_events(keysDown(),
                                                                              NDS_KEYPAD_BITS, NDS_KEYPAD_BITS_SIZE,
                                                                              morpheus::core::InputState::DOWN);
        std::vector<morpheus::core::InputEvent> held_events = to_input_events(keysHeld(),
                                                                              NDS_KEYPAD_BITS, NDS_KEYPAD_BITS_SIZE,
                                                                              morpheus::core::InputState::HELD);
        std::vector<morpheus::core::InputEvent> up_events = to_input_events(keysUp(),
                                                                            NDS_KEYPAD_BITS, NDS_KEYPAD_BITS_SIZE,
                                                                            morpheus::core::InputState::UP);

        std::vector<core::InputEvent> input_events;

        input_events.insert(input_events.end(), down_events.begin(), down_events.end());
        input_events.insert(input_events.end(), held_events.begin(), held_events.end());
        input_events.insert(input_events.end(), up_events.begin(), up_events.end());

        for(core::InputEvent input_event : input_events){
            m_root->received_input(input_event);
        }

        swiWaitForVBlank();

        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
}

morpheus::core::Error morpheus::nds::NdsMainLoop::platform_init() {
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE);

    return morpheus::core::Error::OK;
}

morpheus::core::InputEvent
morpheus::nds::NdsMainLoop::to_input_event(uint32_t inputs,
                                           uint16_t keypad_bit,
                                           morpheus::core::InputState input_state) {
    morpheus::core::InputButton input_button = morpheus::core::InputButton::NONE;
    morpheus::core::InputEvent input_event{};

    if((inputs & keypad_bit) != 0) {
        // TODO(Bobby): Find out better way to prevent code duplication of nds/gba common key code blocks
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
            case KEY_X:
                input_button = morpheus::core::InputButton::X;
                break;
            case KEY_Y:
                input_button = morpheus::core::InputButton::Y;
                break;
            case KEY_TOUCH:
                input_button = morpheus::core::InputButton::TOUCH;
                break;
            case KEY_LID:
                input_button = morpheus::core::InputButton::LID;
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
