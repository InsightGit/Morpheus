//
// Created by bobby on 15/06/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

int main() {
    auto main_loop = std::shared_ptr<morpheus::core::MainLoop>(morpheus::utils::construct_appropriate_main_loop());

    auto tiled_background_base = std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase>(
            morpheus::utils::construct_appropriate_tiled_background_8bpp(false, 0, nullptr, nullptr, main_loop.get(),
                                                                         1, 1, false));

    auto streaming_background_base = std::unique_ptr<morpheus::core::gfx::StreamingBackgroundBase>(
            new morpheus::core::gfx::StreamingBackgroundBase(tiled_background_base.get(),
                                                             morpheus::core::gfx::Vector2(30, 20)));

    //streaming_background_base.

    main_loop->game_loop();
}
