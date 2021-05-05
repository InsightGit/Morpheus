//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_PLAYER_HPP
#define HAYAI_PLAYER_HPP

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#else
#error Unsupported platform!
#endif

#include "playerleftarms0.h"
#include "playerleftarms1.h"
#include "playerleftarms2.h"
#include "playerrightarms0.h"
#include "playerrightarms1.h"
#include "playerrightarms2.h"

namespace hayai {
    class Player {
    public:
        Player(std::shared_ptr<morpheus::core::MainLoop> main_loop);
    private:
        std::unique_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
    };
}

#endif //HAYAI_PLAYER_HPP
