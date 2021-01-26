//
// Created by bobby on 23/01/2021.
//

#include "tiled_background_base.hpp"

morpheus::core::gfx::TiledBackgroundBase::TiledBackgroundBase(unsigned int background_num,
                                                              unsigned int cbb_num, unsigned int sbb_num) {
    m_background_num = std::min(3u, background_num);
    m_cbb_num = std::min(3u, cbb_num);
    m_sbb_num = std::min(31u, sbb_num);
}

