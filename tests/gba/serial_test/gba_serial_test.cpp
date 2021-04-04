//
// Created by bobby on 02/04/2021.
//

#include <gba/gba.hpp>

class InputNode : public {
public:

};

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> main_loop(
            new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::ON));

    tte_init_se(0, BG_CBB(2) | BG_SBB(2), 0, CLR_YELLOW, 14, nullptr, nullptr);

    //main_loop->get_default_communication_channel().

    std::cout << "Test\n";

    main_loop->game_loop();
}
