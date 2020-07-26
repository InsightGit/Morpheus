//
// Created by bobby on 25/07/2020.
//

#include "gba_main_loop.hpp"

morpheus::core::Error morpheus::gba::GbaMainLoop::platform_init() {
    irqInit();
    irqEnable(IRQ_VBLANK);

    REG_IME = 1;

    return morpheus::core::OK;
}

morpheus::core::Error morpheus::gba::GbaMainLoop::game_loop() {
    while(true) {
        VBlankIntrWait();
    }

    return morpheus::core::OK;
}

