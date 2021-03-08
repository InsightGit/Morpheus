#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#else
#error unsupported platform!
#endif

#include "puzzler/main_game_scene.hpp"
#include "puzzler/main_menu_scene.hpp"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop(false, false));
    std::shared_ptr<puzzler::Scene> current_scene(/*new puzzler::MainGameScene(main_loop.get())*/new puzzler::MainMenuScene(main_loop.get()));

    //std::cout << "All ready!\n";
    main_loop->send_to_debug_window("All ready!");

    current_scene->setup();

    main_loop->set_root(std::static_pointer_cast<morpheus::core::Node>(current_scene));

    main_loop->game_loop();
}
