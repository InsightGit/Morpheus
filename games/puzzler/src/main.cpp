#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#else
#error unsupported platform!
#endif

#include "puzzler/main_game_scene.hpp"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop(false, false));
    std::shared_ptr<puzzler::MainGameScene> main_game_scene(new puzzler::MainGameScene(main_loop));

    //std::cout << "All ready!\n";
    main_loop->send_to_debug_window("All ready!");

    main_game_scene->setup();

    main_loop->set_root(main_game_scene);

    main_loop->game_loop();
}
