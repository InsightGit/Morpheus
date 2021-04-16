//
// Created by bobby on 25/07/2020.
//

#include "gba_main_loop.hpp"

morpheus::gba::GbaMainLoop::GbaMainLoop(morpheus::gba::DebugConsoleMode debug_console_mode) :
                            morpheus::core::MainLoop(new morpheus::gba::gfx::GbaBlendingController(),
                                                     new MultiplayerSerialCommunication(),
                                                     new morpheus::gba::gfx::GbaMosaicController(),
                                                     new GbaNoCashDebugController(),
                                                     select_appropriate_save_manager()) {
    #ifdef NDEBUG
        bool debug = false;
    #else
        bool debug = true;
    #endif

    switch (debug_console_mode) {
        case DebugConsoleMode::ON:
            setup_debug_console(true);
            break;
        case DebugConsoleMode::USE_DEFAULT:
            if(debug) {
                setup_debug_console(true);
            }
            break;
        case DebugConsoleMode::NOCASH_ONLY:
            setup_debug_console(false);

            break;
        case DebugConsoleMode::OFF:
            break;
    }
}


morpheus::gba::GbaMainLoop::~GbaMainLoop() {
    for(void *pointer : m_obj_buffer) {
        delete static_cast<OBJ_ATTR *>(pointer);
    }
}

void morpheus::gba::GbaMainLoop::disable_window(morpheus::core::gfx::WindowType window_type) {
    switch(window_type) {
        case morpheus::core::gfx::WindowType::WINDOW_0:
            m_windows_to_enable &= ~DCNT_WIN0;
            break;
        case morpheus::core::gfx::WindowType::WINDOW_1:
            m_windows_to_enable &= ~DCNT_WIN1;
            break;
        case morpheus::core::gfx::WindowType::WINDOW_OBJ:
            m_windows_to_enable &= ~DCNT_WINOBJ;
            break;
        case morpheus::core::gfx::WindowType::WINDOW_OUT:
            // intentionally blank: window out is just the lack of being in a window
            break;
    }
}

void morpheus::gba::GbaMainLoop::enable_background(unsigned int background_num) {
    switch (background_num) {
        case 0:
            m_backgrounds_to_enable |= DCNT_BG0;
            break;
        case 1:
            m_backgrounds_to_enable |= DCNT_BG1;
            break;
        case 2:
            m_backgrounds_to_enable |= DCNT_BG2;
            break;
        case 3:
            m_backgrounds_to_enable |= DCNT_BG3;
            break;
    }

    if(m_platform_inited) {
        REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | m_backgrounds_to_enable | m_windows_to_enable;
    }
}

void morpheus::gba::GbaMainLoop::enable_window(morpheus::core::gfx::WindowType window_type) {
    switch(window_type) {
        case morpheus::core::gfx::WindowType::WINDOW_0:
            m_windows_to_enable |= DCNT_WIN0;
            break;
        case morpheus::core::gfx::WindowType::WINDOW_1:
            m_windows_to_enable |= DCNT_WIN1;
            break;
        case morpheus::core::gfx::WindowType::WINDOW_OBJ:
            m_windows_to_enable |= DCNT_WINOBJ;
            break;
        case morpheus::core::gfx::WindowType::WINDOW_OUT:
            // intentionally blank: window out is just the lack of being in a window
            break;
    }

    if(m_platform_inited) {
        REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | DCNT_BG0 | m_backgrounds_to_enable | m_windows_to_enable;
    }
}

[[noreturn]] morpheus::core::Error morpheus::gba::GbaMainLoop::game_loop() {
    platform_init();

    while(true) {
        key_poll();

        std::vector<core::InputEvent> down_events = to_input_events(key_hit(KEY_FULL), GBA_KEYPAD_BITS,
                                                                    GBA_KEYPAD_BITS_SIZE, core::InputState::DOWN);
        std::vector<core::InputEvent> held_events = to_input_events(key_is_down(KEY_FULL), GBA_KEYPAD_BITS,
                                                                    GBA_KEYPAD_BITS_SIZE, core::InputState::HELD);
        std::vector<core::InputEvent> up_events = to_input_events(key_released(KEY_FULL), GBA_KEYPAD_BITS,
                                                                  GBA_KEYPAD_BITS_SIZE, core::InputState::UP);

        std::vector<core::InputEvent> input_events;

        input_events.insert(input_events.end(), down_events.begin(), down_events.end());
        input_events.insert(input_events.end(), held_events.begin(), held_events.end());
        input_events.insert(input_events.end(), up_events.begin(), up_events.end());

        if(input_events.size() > 0) {
            m_last_input_size = input_events.size() * m_cycle_time;
        }

        for(std::shared_ptr<core::ControlReciever> &control_reciever : m_control_recievers) {
            for(core::InputEvent &event : input_events) {
                control_reciever->input(event);
            }

            control_reciever->update(m_cycle_time);
        }

        for(std::shared_ptr<core::gfx::SpriteBase> &sprite : m_sprites) {
            for(core::InputEvent input_event : input_events) {
                sprite->input(input_event);
            }

            sprite->update(m_cycle_time);
        }

        /*m_sprites.erase(std::remove_if(m_sprites.begin(), m_sprites.end(),
                                               [](const std::shared_ptr<core::gfx::SpriteBase> &sprite) {
            return sprite.get() == nullptr;
        }), m_sprites.end());*/

        for(unsigned int i = 0; m_sprites.size() > i; ++i) {
            m_sprites[i]->draw(m_obj_buffer, i);

            oam_copy(oam_mem + (OBJ_ATTR_SIZE * i), static_cast<OBJ_ATTR *>(m_obj_buffer[i]), 1);
        }

        if(m_debug_stream != nullptr) {
            m_debug_stream->refresh_and_print();
        }

        ++m_cycle_time;

        if(m_cycle_time >= 60) {
            m_cycle_time = 0;
        }

        set_supplementary_seed(static_cast<int>(m_cycle_time) + m_obj_buffer.size() + m_last_input_size);

        get_default_communication_channel()->update();

        mmFrame();

        VBlankIntrWait();
    }
    //return morpheus::core::OK;
}

morpheus::core::Error morpheus::gba::GbaMainLoop::platform_init() {
    irq_init(nullptr);
    irq_enable(eIrqIndex::II_VBLANK);

    REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | m_backgrounds_to_enable | m_windows_to_enable;
    REG_IME = 1;

    if(is_affine()) {
        switch(get_affine_mode()) {
            case morpheus::core::gfx::AffineMode::MIXED_AFFINE:
                REG_DISPCNT |= DCNT_MODE1;
                break;
            case morpheus::core::gfx::AffineMode::FULL_AFFINE:
                REG_DISPCNT |= DCNT_MODE2;
                break;
        }
    } else {
        REG_DISPCNT |= DCNT_MODE0;
    }

    for(int i = 0; GBA_MAX_SPRITES > i; ++i) {
        m_obj_buffer.push_back(new OBJ_ATTR());
    }

    oam_init(static_cast<OBJ_ATTR *>(m_obj_buffer[0]), 128);

    m_platform_inited = true;

    irq_add(II_VBLANK, mmVBlank);
    irq_enable(II_VBLANK);

    return morpheus::core::Error::OK;
}

morpheus::core::InputEvent morpheus::gba::GbaMainLoop::to_input_event(uint32_t inputs,
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

void morpheus::gba::GbaMainLoop::DebugStream::refresh_and_print() {
    if(!str().empty()) {
        if(m_main_loop->m_using_tte) {
            tte_write(str().c_str());
        }

        nocash_puts(str().c_str());

        str("");
        clear();
    }
}

void morpheus::gba::GbaMainLoop::setup_debug_console(bool use_tte) {
    //tte_init_se_default(0, BG_CBB(0) | BG_SBB(31));

    if(use_tte) {
        tte_init_se(0, BG_CBB(2) | BG_SBB(31), 0, CLR_WHITE, 14, nullptr, nullptr);

        enable_background(0);
    }

    m_debug_stream = std::unique_ptr<DebugStream>(new DebugStream(this));

    std::cout.rdbuf(m_debug_stream.get()->rdbuf_string_stream());

    std::cout << "morpheus debug\n";
}

void morpheus::gba::GbaMainLoop::clear_obj_vram() {
    //memcpy16(oam_mem, 0, 256);

    memset16(reinterpret_cast<void *>(MEM_VRAM), 0, 98304);
}

void morpheus::gba::GbaMainLoop::disable_affine() {
    set_affine(false);

    if(m_platform_inited) {
        REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE0 | m_backgrounds_to_enable | m_windows_to_enable;
    }
}

void morpheus::gba::GbaMainLoop::enable_affine(morpheus::core::gfx::AffineMode affine_mode) {
    set_affine(true);
    set_affine_mode(affine_mode);

    if(m_platform_inited) {
        switch(affine_mode) {
            case morpheus::core::gfx::AffineMode::MIXED_AFFINE:
                REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE1 | m_backgrounds_to_enable | m_windows_to_enable;
                break;
            case morpheus::core::gfx::AffineMode::FULL_AFFINE:
                REG_DISPCNT = DCNT_OBJ | DCNT_OBJ_1D | DCNT_MODE2 | m_backgrounds_to_enable | m_windows_to_enable;
                break;
        }
    }
}

morpheus::core::SaveManager *morpheus::gba::GbaMainLoop::select_appropriate_save_manager() {
#ifdef GBA_EEPROM_SAVE
    // TODO(Bobby): Implement EEPROM save manager
    return nullptr;
#else
    return new morpheus::gba::GbaSramSaveManager();
#endif
}

/*void morpheus::gba::GbaMainLoop::clear_vram() {
    memset16(pal_obj_mem, 0, 512);
    memset16(pal_bg_mem, 0, 512);

    memset16(tile_mem, 0, 2048);
    memset16(tile8_mem, 0, 2048);

    memset16(tile_mem_obj, 0, 2048);
    memset16(tile8_mem_obj, 0, 2048);

    memset16(se_mem, 0, 4096);
}*/
