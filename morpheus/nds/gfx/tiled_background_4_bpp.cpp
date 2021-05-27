//
// Created by bobby on 26/01/2021.
//

#include "tiled_background_4_bpp.hpp"

void morpheus::nds::gfx::TiledBackground4Bpp::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                         const unsigned short *palette, const unsigned int pal_len,
                                                         const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         morpheus::core::gfx::TiledBackgroundSize size) {
    load_from_array(tiles, tiles_len, tile_map, tile_map_len, size);

    if(is_using_sub_display()) {
        dmaCopy(palette, BG_PALETTE_SUB, pal_len);
    } else {
        dmaCopy(palette, BG_PALETTE, pal_len);
    }
}

void morpheus::nds::gfx::TiledBackground4Bpp::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                              const unsigned short *tile_map,
                                                              const unsigned int tile_map_len,
                                                              morpheus::core::gfx::TiledBackgroundSize size) {
    load_from_array(tile_map, tile_map_len, size);

    dmaCopy(tiles, bgGetGfxPtr(get_background_reference_num()), tiles_len);
}

void morpheus::nds::gfx::TiledBackground4Bpp::array_load(const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         morpheus::core::gfx::TiledBackgroundSize size) {
    set_background_size(size);

    if(is_affine()) {
        bgSetRotateScale(get_background_reference_num(), get_rotation(), get_scale().get_x(),
                         get_scale().get_y());

        // resets 8bpp color mode bit if set
        *get_background_register() &= 0xFF7F;
    }

    dmaCopy(tile_map, bgGetMapPtr(get_background_reference_num()), tile_map_len);
}
