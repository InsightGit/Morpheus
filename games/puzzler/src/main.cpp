#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#else
#error unsupported platform!
#endif

#include "puzzler/scene_manager.hpp"
#include "puzzler/main_game_scene.hpp"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop(false, false));
    std::shared_ptr<puzzler::SceneManager> current_scene(new puzzler::SceneManager(main_loop.get()));
    //std::shared_ptr<puzzler::Scene> current_scene(new puzzler::MainGameScene(main_loop.get(), 0));

    //std::cout << "All ready!\n";
    main_loop->get_no_cash_debug_controller()->send_to_debug_window("All ready!");

    //current_scene->setup();

    main_loop->add_control_reciever(current_scene);

    main_loop->game_loop();
}
