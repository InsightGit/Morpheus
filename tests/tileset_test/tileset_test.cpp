//
// Created by bobby on 08/01/2021.
//

#include <core/core.hpp>

#include "region_map2.h"
#include "region_map.h"
#include "brin.h"

#ifdef _NDS
    #include <nds/nds.hpp>
#elif _GBA
    #include <gba/gba.hpp>
#else
    #error Unknown platform!
#endif

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    // Background 2 is only for the DS's sub GPU
    std::array<std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase>, 3> backgrounds;

    backgrounds[0].reset(morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 1, nullptr, nullptr,
                                                                                      main_loop.get(), 1, 1));
    backgrounds[1].reset(morpheus::utils::construct_appropriate_tiled_background_4bpp(false, 2, nullptr, nullptr,
                                                                                      main_loop.get(), 2, 1));

    #ifdef _NDS
        std::cout << "Initing TiledBackground class\n";

        backgrounds[2].reset(new morpheus::nds::gfx::TiledBackground4Bpp(false, true, 2u, nullptr, nullptr,
                                                                         std::static_pointer_cast<
                                                                         morpheus::nds::NdsMainLoop>(main_loop).get(),
                                                                         2u, 2u));

        backgrounds[2]->load_from_array(reinterpret_cast<const unsigned int*>(brinTiles), brinTilesLen, brinPal,
                                        brinPalLen, brinMap, brinMapLen,
                                        morpheus::core::gfx::TiledBackgroundSize::BG_64x32);
    #endif

    std::shared_ptr<morpheus::utils::BackgroundTestControls> controls(
                                                       new morpheus::utils::BackgroundTestControls(backgrounds[0]));

    std::cout << "Loading TiledBackground class\n";

    backgrounds[0]->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                                    region_mapMap, region_mapMapLen * 4,
                                    morpheus::core::gfx::TiledBackgroundSize::BG_32x32);


    std::cout << "All ready!\n";

    main_loop->add_control_reciever(controls);

    main_loop->game_loop();
}
