//
// Created by bobby on 25/05/2021.
//

#ifndef MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP

#include "tiled_background_base.hpp"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class StreamingBackgroundSize {
                BG_64x128,
            };

            class StreamingBackgroundBase : public core::gfx::TiledBackgroundBase {
            public:
                StreamingBackgroundBase(bool affine, unsigned int backgroundNum,
                                        morpheus::core::gfx::BlendingController *blendingController,
                                        morpheus::core::gfx::MosaicController *mosaicController,
                                        unsigned int cbbNum, unsigned int sbbNum);

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short **tile_maps, StreamingBackgroundSize size);
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_STREAMING_BACKGROUND_HPP
