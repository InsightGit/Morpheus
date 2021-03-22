//
// Created by bobby on 25/11/2020.
//

#include "nds_main_loop.hpp"

PrintConsole *morpheus::nds::NdsMainLoop::debug_print_console = nullptr;

morpheus::nds::NdsMainLoop::NdsMainLoop(morpheus::nds::DebugConsoleMode debug_console_mode) :
            morpheus::core::MainLoop(new morpheus::nds::gfx::NdsBlendingController(false)) {
    #ifdef NDEBUG
        bool debug = false;
    #else
        bool debug = true;
    #endif

    m_sub_blending_controller.reset(new morpheus::nds::gfx::NdsBlendingController(true));

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    switch(debug_console_mode) {
        case DebugConsoleMode::USE_DEFAULT_MAIN:
            if(debug) {
                setup_debug_console(true);
            }
            break;
        case DebugConsoleMode::USE_DEFAULT_SUB:
            if(debug) {
                setup_debug_console(false);
            }
            break;
        case DebugConsoleMode::ON_MAIN:
            setup_debug_console(true);
            break;
        case DebugConsoleMode::ON_SUB:
            setup_debug_console(false);
            break;
        case DebugConsoleMode::OFF:
            break;
        default:
            assert(false);
    }
}

void morpheus::nds::NdsMainLoop::disable_window(morpheus::core::gfx::WindowType window_type) {
    //
}

void morpheus::nds::NdsMainLoop::enable_background(unsigned int background_reference_num) {
    //std::cout << "Showing " << background_reference_num << "\n";

    bgShow(background_reference_num);
}

void morpheus::nds::NdsMainLoop::enable_window(morpheus::core::gfx::WindowType window_type) {
    //
}

morpheus::core::Error morpheus::nds::NdsMainLoop::game_loop() {
    platform_init();

    int iteration = 0;

    while(true) {
        reset_to_debug_print_console();

        if(m_root != nullptr) {
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

            if(!input_events.empty()) {
                m_last_input_size = input_events.size() * m_cycle_time;
            }

            for(core::InputEvent input_event : input_events) {
                m_root->received_input(input_event);
            }

            m_root->received_update(m_cycle_time);

            oamClear(&oamMain, 0, 0);
            oamClear(&oamSub, 0, 0);

            // TODO(Bobby): Fix unneeded argument 1 problem
            std::vector<void*> filler;

            set_supplementary_seed(static_cast<int>(m_cycle_time) +
                                    m_root->draw(filler, 0) + m_last_input_size);
        }

        ++iteration;

        /*iprintf("\x1b[21;0H      Morpheus Debug");
        iprintf("\x1b[22;0HIterations = %d", iteration);*/

        ++m_cycle_time;

        if(m_cycle_time >= 60) {
            m_cycle_time = 0;
        }

        // to insure the randomness of the random number generator
        get_random_number(0, 0);

        swiWaitForVBlank();

        oamUpdate(&oamMain);
        oamUpdate(&oamSub);

        bgUpdate();
    }
}

morpheus::core::Error morpheus::nds::NdsMainLoop::platform_init() {
    return morpheus::core::Error::OK;
}

void morpheus::nds::NdsMainLoop::setup_debug_console(bool use_main_display) {
    if(use_main_display) {
        vramSetBankA(VRAM_A_MAIN_BG);
        videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
    } else {
        vramSetBankC(VRAM_C_SUB_BG);
        videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);
    }

    debug_print_console = consoleInit(nullptr, 0, BgType_Text4bpp, BgSize_T_256x256, 22, 3, use_main_display, true);
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

void morpheus::nds::NdsMainLoop::clear_obj_vram() {
    oamClear(&oamMain, 0, 0);
    oamClear(&oamSub, 0, 0);
}

/*void morpheus::nds::NdsMainLoop::clear_vram() {
    //
}*/
