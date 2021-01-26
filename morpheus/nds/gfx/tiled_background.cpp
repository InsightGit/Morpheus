//
// Created by bobby on 21/01/2021.
//

#include "tiled_background.hpp"

morpheus::nds::gfx::TiledBackground::TiledBackground(bool use_sub_display, unsigned int background_num,
                                                     std::shared_ptr<NdsMainLoop> main_loop, bool is_8bpp,
                                                     unsigned int cbb_num, unsigned int sbb_num) :
                                     morpheus::core::gfx::TiledBackgroundBase(background_num, cbb_num, sbb_num) {
    m_main_loop = main_loop;
    m_use_sub_display = use_sub_display;

    if(is_8bpp) {
        m_background_type = BgType_Text8bpp;
    } else {
        m_background_type = BgType_Text4bpp;
    }
}

void morpheus::nds::gfx::TiledBackground::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                          const unsigned short *palette, const unsigned int pal_len,
                                                          const unsigned short *tile_map,
                                                          const unsigned int tile_map_len,
                                                          morpheus::core::gfx::TiledBackgroundSize size) {
    load_from_array(tiles, tiles_len, palette, tile_map_len, size);

    dmaCopy(palette, BG_PALETTE, pal_len);
}

void morpheus::nds::gfx::TiledBackground::load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                                          const unsigned short *tile_map,
                                                          const unsigned int tile_map_len,
                                                          morpheus::core::gfx::TiledBackgroundSize size) {
    std::cout << "loading within object\n";
    switch(size) {
        case morpheus::core::gfx::TiledBackgroundSize::BG_32x32:
            m_background_size = BgSize_T_256x256;
            break;
        case morpheus::core::gfx::TiledBackgroundSize::BG_32x64:
            m_background_size = BgSize_T_256x512;
            break;
        case morpheus::core::gfx::TiledBackgroundSize::BG_64x32:
            m_background_size = BgSize_T_512x256;
            break;
        case morpheus::core::gfx::TiledBackgroundSize::BG_64x64:
            m_background_size = BgSize_T_512x512;
            break;
    }

    if(m_background_reference_num < 0) {
        if(m_use_sub_display) {
            vramSetBankC(VRAM_C_SUB_BG);

            m_background_reference_num =
                    bgInitSub(get_background_num(), m_background_type, m_background_size, get_sbb_num(),
                              get_cbb_num());
        } else {
            vramSetBankA(VRAM_A_MAIN_BG);

            m_background_reference_num =
                    bgInit(get_background_num(), m_background_type, m_background_size, get_sbb_num(),
                           get_cbb_num());
        }

        m_main_loop->enable_background(m_background_reference_num);
    }

    dmaCopy(tiles, bgGetGfxPtr(m_background_reference_num), tiles_len);
    dmaCopy(tile_map, bgGetMapPtr(m_background_reference_num), tile_map_len);
}

void morpheus::nds::gfx::TiledBackground::update_scroll() {
    morpheus::core::gfx::Vector2 position = get_scroll();

    bgSetScroll(m_background_reference_num, position.get_x(), position.get_y());
}
