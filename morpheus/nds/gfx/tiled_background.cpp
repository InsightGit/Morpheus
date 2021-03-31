//
// Created by bobby on 21/01/2021.
//

#include "tiled_background.hpp"

morpheus::nds::gfx::TiledBackground::TiledBackground(bool use_sub_display, unsigned int background_num,
                                                     NdsBlendingController *blending_controller,
                                                     NdsMosaicController *mosaic_controller,
                                                     NdsMainLoop *main_loop,
                                                     unsigned int cbb_num, unsigned int sbb_num) :
                                     morpheus::core::gfx::TiledBackgroundBase(background_num, blending_controller,
                                                                              mosaic_controller, cbb_num, sbb_num) {
    m_main_loop = main_loop;
    m_use_sub_display = use_sub_display;
}


void morpheus::nds::gfx::TiledBackground::update_scroll() {
    morpheus::core::gfx::Vector2 position = get_scroll();

    bgSetScroll(m_background_reference_num, position.get_x(), position.get_y());
}

void morpheus::nds::gfx::TiledBackground::set_background_size(morpheus::core::gfx::TiledBackgroundSize size) {
    //std::cout << "setting background size\n";
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
}

void morpheus::nds::gfx::TiledBackground::init_background_reference_num(BgType background_type) {
    if(m_background_reference_num < 0) {
        if(m_use_sub_display) {
            vramSetBankC(VRAM_C_SUB_BG);

            m_background_reference_num =
                    bgInitSub(static_cast<int>(get_background_num()), background_type, m_background_size,
                              static_cast<int>(get_sbb_num()),static_cast<int>(get_cbb_num()));
        } else {
            vramSetBankA(VRAM_A_MAIN_BG);

            m_background_reference_num =
                    bgInit(static_cast<int>(get_background_num()), background_type, m_background_size,
                           static_cast<int>(get_sbb_num()),static_cast<int>(get_cbb_num()));
        }

        mosaic_state_updated();

        m_main_loop->enable_background(m_background_reference_num);
    }
}

void morpheus::nds::gfx::TiledBackground::mosaic_state_updated() {
    if(is_mosaic() && m_background_reference_num >= 0) {
        bgMosaicEnable(m_background_reference_num);
    } else if(m_background_reference_num >= 0) {
        bgMosaicDisable(m_background_reference_num);
    }
}
