//
// Created by bobby on 23/01/2021.
//

#include "tiled_background_base.hpp"

morpheus::core::gfx::TiledBackgroundBase::TiledBackgroundBase(bool affine, unsigned int background_num,
                                                              BlendingController *blending_controller,
                                                              MosaicController *mosaic_controller,
                                                              unsigned int cbb_num, unsigned int sbb_num) {
    m_affine = affine;
    m_background_num = std::min(3u, background_num);
    m_blending_controller = blending_controller;
    m_cbb_num = std::min(3u, cbb_num);
    m_mosaic_controller = mosaic_controller;
    m_sbb_num = std::min(31u, sbb_num);
}

