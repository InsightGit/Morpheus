#include "hayai/level.hpp"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    std::shared_ptr<hayai::Scene> scene(new hayai::Level(main_loop), hayai::SceneDeleter());

    main_loop->add_control_reciever(scene);

    main_loop->game_loop();
}
