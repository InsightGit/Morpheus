//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_LEVEL_HPP
#define HAYAI_LEVEL_HPP

#include "scene.hpp"

#include "test_map_1.h"

namespace hayai {
    class Level : public Scene {
    public:
        Level(std::shared_ptr<morpheus::core::MainLoop> main_loop);
    private:
        std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase> m_level_background;
    };
}

#endif //HAYAI_LEVEL_HPP
