//
// Created by bobby on 05/02/2021.
//

#include <string>

#include <gba/gba.hpp>

#include "soundbank.h"
#include "soundbank_bin.h"

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> main_loop(
                                                new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::ON));

    std::string before_string;
    std::string test_string = "gamer-abcd";
    const char *test_cstring = test_string.c_str();

    char *sram_pos = reinterpret_cast<char*>(0x0E000000);

    while(*sram_pos != 0x00) {
        before_string += *sram_pos;

        ++sram_pos;
    }


    for(unsigned int i = 0; test_string.length() > i; ++i) {
        *(reinterpret_cast<char*>(0x0E000000 + i)) = *(test_cstring + i);
    }

    std::shared_ptr<morpheus::core::audio::MaxModSfx> max_mod_sfx(
            morpheus::utils::construct_appropriate_max_mod_sfx(SFX_EXAMPLESFX,
                                        const_cast<void *>(static_cast<const void*>(&soundbank_bin)), 4));

    max_mod_sfx->start_effect(false);

    std::cout << "before: " << before_string  << "\nafter: " << test_string << "\n";

   /* // SRAM starts at 0x0E000000 and goes to 0x0E00FFFF
    for(int *sram = reinterpret_cast<int *>(0x0E000000); reinterpret_cast<int *>(0x0E00FFFF) >= sram; ++sram) {
        *(sram) = 0x1;
    }*/

    main_loop->game_loop();
}
