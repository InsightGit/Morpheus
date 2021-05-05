//
// Created by bobby on 04/05/2021.
//

#include "player.hpp"

hayai::Player::Player(std::shared_ptr<morpheus::core::MainLoop> main_loop) {
    m_sprite_base.reset(morpheus::utils::construct_appropriate_sprite_4bpp(false,
                                                                           main_loop->get_blending_controller(),
                                                                           main_loop->get_mosaic_controller()));

    m_sprite_base->load_into_palette()

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array()
    #elif _NDS
    #endif
}
