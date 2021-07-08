//
// Created by bobby on 26/01/2021.
//

#include "morpheus/nds/gfx/tiled_background_4_bpp.hpp"

void morpheus::nds::gfx::TiledBackground4Bpp::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                         const unsigned short *palette, const unsigned int pal_len,
                                                         const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         const morpheus::core::gfx::TiledBackgroundSize size,
                                                         const core::gfx::BitUnpacking unpacking_needed,
                                                         const unsigned int palette_offset) {
    if(is_using_sub_display()) {
        dmaCopy(palette, BG_PALETTE_SUB + (2 * palette_offset), pal_len);
    } else {
        dmaCopy(palette, BG_PALETTE + (2 * palette_offset), pal_len);
    }

    load_from_array(tiles, tiles_len, tile_map, tile_map_len, size, unpacking_needed);
}

void morpheus::nds::gfx::TiledBackground4Bpp::array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                                         const unsigned short *tile_map,
                                                         const unsigned int tile_map_len,
                                                         const morpheus::core::gfx::TiledBackgroundSize size,
                                                         const core::gfx::BitUnpacking unpacking_needed) {
    asm_BitUnPackOptions bit_unpacking_options;
    bool ready_for_unpacking = false;

    bit_unpacking_options.source_len = tiles_len;

    load_from_array(tile_map, tile_map_len, size);

    switch(unpacking_needed) {
        case core::gfx::BitUnpacking::NONE:
            break;
        case core::gfx::BitUnpacking::BPP_1_TO_4:
            bit_unpacking_options.source_bit_width = 1;
            bit_unpacking_options.dest_bit_width = 4;
            bit_unpacking_options.offset_plus_zero_data_flag = 0;

            ready_for_unpacking = true;

            nocashMessage("set to BPP_1_TO_4!");
            break;
        case core::gfx::BitUnpacking::BPP_1_TO_8:
            nocashMessage("Incorrect bit conversion specified! BPP_1_TO_8 was tried on a 4bpp background!");
            break;
        case core::gfx::BitUnpacking::BPP_4_TO_8:
            nocashMessage("Incorrect bit conversion specified! BPP_4_TO_8 was tried on a 4bpp background!");
            break;
    }

    if(ready_for_unpacking) {
        asm_BitUnPack(tiles, bgGetGfxPtr(get_background_reference_num()), &bit_unpacking_options);
    } else {
        dmaCopy(tiles, bgGetGfxPtr(get_background_reference_num()), tiles_len);
    }
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

    if(tile_map_len > 0 && tile_map != nullptr) {
        dmaCopy(tile_map, bgGetMapPtr(get_background_reference_num()), tile_map_len);
    }
}
