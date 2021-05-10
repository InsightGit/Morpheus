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

    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background0;
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background1;
    // only for NDS' sub GPU
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background2;

    #ifdef _GBA
        background0.reset(new morpheus::gba::gfx::TiledBackground(false, 1, nullptr, nullptr,
                                                                    std::static_pointer_cast<
                                                                        morpheus::gba::GbaMainLoop>(main_loop),
                                                                 true, 1, 0));
        background1.reset(new morpheus::gba::gfx::TiledBackground(false, 2,
                                                              std::static_pointer_cast<
                                                                      morpheus::gba::GbaMainLoop>(main_loop),
                                                              false, 2, 1));
    #elif _NDS
        std::cout << "Initing TiledBackground class\n";

        defaultExceptionHandler();

        background0.reset(new morpheus::nds::gfx::TiledBackground8Bpp(false, 1,
                                                                 std::static_pointer_cast<
                                                                    morpheus::nds::NdsMainLoop>(main_loop),
                                                                    1, 1));
        background1.reset(new morpheus::nds::gfx::TiledBackground8Bpp(false, 2,
                                                                  std::static_pointer_cast<
                                                                          morpheus::nds::NdsMainLoop>(main_loop),
                                                                  2, 2));
        background2.reset(new morpheus::nds::gfx::TiledBackground4Bpp(true, 2,
                                                                  std::static_pointer_cast<
                                                                          morpheus::nds::NdsMainLoop>(main_loop),
                                                                  2, 2));

        background2->load_from_array(reinterpret_cast<const unsigned int*>(brinTiles), brinTilesLen, brinPal, brinPalLen,
                                 brinMap, brinMapLen, morpheus::core::gfx::TiledBackgroundSize::BG_64x32);
    #endif

    std::shared_ptr<morpheus::utils::BackgroundTestControls> controls(
                                                           new morpheus::utils::BackgroundTestControls(background1));

    std::cout << "Loading TiledBackground class\n";

    background0->load_from_array(region_mapTiles, region_mapTilesLen, region_mapPal, region_mapPalLen,
                           region_mapMap, region_mapMapLen * 4,
                           morpheus::core::gfx::TiledBackgroundSize::BG_32x32);


    std::cout << "All ready!\n";

    main_loop->set_root(controls);

    main_loop->game_loop();
}
