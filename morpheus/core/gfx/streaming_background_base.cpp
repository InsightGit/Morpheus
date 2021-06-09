//
// Created by bobby on 25/05/2021.
//

#include "streaming_background_base.hpp"

morpheus::core::gfx::StreamingBackgroundBase::StreamingBackgroundBase(bool affine, unsigned int backgroundNum,
                                                                      morpheus::core::gfx::BlendingController *blendingController,
                                                                      morpheus::core::gfx::MosaicController *mosaicController,
                                                                      unsigned int cbbNum, unsigned int sbbNum)
        : TiledBackgroundBase(affine, backgroundNum, blendingController, mosaicController, cbbNum, sbbNum, false) {

}

void
morpheus::core::gfx::StreamingBackgroundBase::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                              const unsigned short *palette, const unsigned int pal_len,
                                                              const unsigned short **tile_maps,
                                                              morpheus::core::gfx::StreamingBackgroundSize size) {

}
