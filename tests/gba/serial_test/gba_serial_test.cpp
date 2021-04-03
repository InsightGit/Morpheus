//
// Created by bobby on 02/04/2021.
//

#include <gba/gba.hpp>

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> main_loop(
            new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::ON));

    tte_init_se(0, )

    //main_loop->get_default_communication_channel().

    std::cout << "Test\n";

    main_loop->game_loop();
}
