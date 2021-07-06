//
// Created by bobby on 26/01/2021.
//

#include "morpheus/nds/gfx/tiled_background_8_bpp.hpp"

void morpheus::nds::gfx::TiledBackground8Bpp::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                         const unsigned short *palette, const unsigned int pal_len,
                                                         const unsigned int palette_id, const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         core::gfx::TiledBackgroundSize size) {
    if(is_using_sub_display()) {
        bgExtPaletteEnableSub();
    } else {
        bgExtPaletteEnable();
    }

    //std::cout << "Loading via extended palette\n" << "into pal_id:" << palette_id << " background num:"
    //          << get_background_num() << "\n";

    array_load(tiles, tiles_len, tile_map, tile_map_len, size);

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

void morpheus::nds::gfx::TiledBackground8Bpp::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                         const unsigned short *palette, const unsigned int pal_len,
                                                         const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         const core::gfx::TiledBackgroundSize size,
                                                         const core::gfx::BitUnpacking unpacking_needed) {
    array_load(tiles, tiles_len, tile_map, tile_map_len, size, unpacking_needed);

    if(is_using_sub_display()) {
        dmaCopy(palette, BG_PALETTE_SUB, pal_len);
    } else {
        dmaCopy(palette, BG_PALETTE, pal_len);
    }
}

void morpheus::nds::gfx::TiledBackground8Bpp::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                         const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         const core::gfx::TiledBackgroundSize size,
                                                         const core::gfx::BitUnpacking unpacking_needed) {
    asm_BitUnPackOptions bit_unpacking_options;
    bool ready_for_unpacking = false;

    array_load(tile_map, tile_map_len, size);

    switch(unpacking_needed) {
        case core::gfx::BitUnpacking::NONE:
            break;
        case core::gfx::BitUnpacking::BPP_1_TO_4:
            nocashMessage("Incorrect bit conversion specified! BPP_1_TO_4 was tried on a 8bpp background!");
            break;
        case core::gfx::BitUnpacking::BPP_1_TO_8:
            bit_unpacking_options.source_bit_width = 1;
            bit_unpacking_options.dest_bit_width = 8;
            bit_unpacking_options.offset_plus_zero_data_flag = 0;

            ready_for_unpacking = true;
            break;
        case core::gfx::BitUnpacking::BPP_4_TO_8:
            bit_unpacking_options.source_bit_width = 4;
            bit_unpacking_options.dest_bit_width = 8;
            bit_unpacking_options.offset_plus_zero_data_flag = 0;

            ready_for_unpacking = true;
            break;
    }

    if(ready_for_unpacking) {
        asm_BitUnPack(tiles, bgGetGfxPtr(get_background_reference_num()), &bit_unpacking_options);
    } else {
        dmaCopy(tiles, bgGetGfxPtr(get_background_reference_num()), tiles_len);
    }

    dmaCopy(tiles, bgGetGfxPtr(get_background_reference_num()), tiles_len);
}

void morpheus::nds::gfx::TiledBackground8Bpp::array_load(const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         morpheus::core::gfx::TiledBackgroundSize size) {
    set_background_size(size);

    if(is_affine()) {
        bgSetRotateScale(get_background_reference_num(), get_rotation(), get_scale().get_x(),
                         get_scale().get_y());

        // triggers 8bpp color mode
        *get_background_register() |= (1 << 7);
    }

    dmaCopy(tile_map, bgGetMapPtr(get_background_reference_num()), tile_map_len);
}
