//
// Created by bobby on 26/01/2021.
//

#include "tiled_background_8_bpp.hpp"

void morpheus::nds::gfx::TiledBackground8Bpp::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                              const unsigned short *palette, const unsigned int pal_len,
                                                              const unsigned int palette_id,
                                                              const unsigned short *tile_map,
                                                              const unsigned int tile_map_len,
                                                              morpheus::core::gfx::TiledBackgroundSize size) {
    if(is_using_sub_display()) {
        bgExtPaletteEnableSub();
    } else {
        bgExtPaletteEnable();
    }

    //std::cout << "Loading via extended palette\n" << "into pal_id:" << palette_id << " background num:"
    //          << get_background_num() << "\n";

    load_from_array(tiles, tiles_len, tile_map, tile_map_len, size);

    if(is_using_sub_display()) {
        vramSetBankH(VRAM_H_LCD);

        dmaCopy(palette, &VRAM_H_EXT_PALETTE[get_background_num()][palette_id], pal_len);

        vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    } else {
        vramSetBankE(VRAM_E_LCD);

        dmaCopy(palette, &VRAM_E_EXT_PALETTE[get_background_num()][palette_id], pal_len);

        vramSetBankE(VRAM_E_BG_EXT_PALETTE);
    }
}

void morpheus::nds::gfx::TiledBackground8Bpp::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
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

void morpheus::nds::gfx::TiledBackground8Bpp::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                              const unsigned short *tile_map,
                                                              const unsigned int tile_map_len,
                                                              morpheus::core::gfx::TiledBackgroundSize size) {
    set_background_size(size);

    if(is_affine()) {
        bgSetRotateScale(get_background_reference_num(), get_rotation(), get_scale().get_x(),
                         get_scale().get_y());
    }

    dmaCopy(tiles, bgGetGfxPtr(get_background_reference_num()), tiles_len);
    dmaCopy(tile_map, bgGetMapPtr(get_background_reference_num()), tile_map_len);
}
