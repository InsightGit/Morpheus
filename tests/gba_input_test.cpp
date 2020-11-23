#include <core/core.hpp>
#include <gba/gba.hpp>

#include "testt.h"

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> gba_main_loop(new morpheus::gba::GbaMainLoop());

    morpheus::gba::gfx::Sprite test_sprite{};

    test_sprite.load_from_array((unsigned short **) testtTiles, testtTilesLen, (unsigned short **) testtPal, testtPalLen,
                        4, 32, 32, 0);

    test_sprite.set_position(64, 64);

    gba_main_loop->set_root(std::shared_ptr<morpheus::gba::gfx::Sprite>(&test_sprite));

    gba_main_loop->game_loop();
}
