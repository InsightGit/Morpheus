//
// Created by bobby on 08/01/2021.
//

#include <core/core.hpp>

#include "region_map.h"

/*#ifdef _NDS
    #include <nds/nds.hpp>
    bool on_ds = true;
#elif _GBA
    #include <gba/gba.hpp>
    bool on_ds = false;
#else
    #error Unknown platform!
#endif*/

#include <gba/gba.hpp>
bool on_ds = false;


int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::OFF)/*morpheus::core::MainLoop::construct_appropriate_main_loop()*/);

    if(on_ds) {
        //defaultExceptionHandler();
    } else {
        morpheus::gba::gfx::Background background(0);

        background.load_from_array(region_mapTiles, region_mapTilesLen, region_mapTileMap, region_mapPal);
    }

    main_loop->game_loop();
}

