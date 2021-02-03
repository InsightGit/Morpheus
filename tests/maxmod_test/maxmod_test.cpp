//
// Created by bobby on 02/02/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    std::shared_ptr<morpheus::core::audio::MaxModMusic> max_mod_music(morpheus::utils::construct_appropriate_max_mod_music());

    main_loop->game_loop();
}
