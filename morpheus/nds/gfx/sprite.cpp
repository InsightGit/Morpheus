//
// Created by bobby on 17/12/2020.
//

#include "sprite.hpp"

morpheus::nds::gfx::Sprite::Sprite(const bool use_sub_display, const SpriteMapping sprite_mapping,
                                   const bool extended_palette) {
    if(use_sub_display) {
        m_current_oam = &oamSub;

        vramSetBankD(VRAM_D_SUB_SPRITE);

    } else {
        m_current_oam = &oamMain;

        vramSetBankB(VRAM_B_MAIN_SPRITE);
    }

    bool is_bitmap = sprite_mapping == SpriteMapping_Bmp_1D_128 ||
                     sprite_mapping == SpriteMapping_1D_256 ||
                     sprite_mapping == SpriteMapping_Bmp_2D_128 ||
                     sprite_mapping == SpriteMapping_Bmp_2D_256;
    OamStatus new_oam_status;

    m_use_extended_palette = extended_palette;

    if(m_use_extended_palette) {
        if(is_bitmap) {
            new_oam_status = OamStatus::ENABLED_EXTENDED_BITMAP;
        } else {
            new_oam_status = OamStatus::ENABLED_EXTENDED;
        }
    } else {
        if(is_bitmap) {
            new_oam_status = OamStatus::ENABLED_BITMAP;
        } else {
            new_oam_status = OamStatus::ENABLED;
        }
    }

    if(OAM_STATUS != new_oam_status) {
        OAM_STATUS = new_oam_status;

        oamInit(m_current_oam, sprite_mapping, m_use_extended_palette);
    }
}

morpheus::nds::gfx::Sprite::~Sprite() {
    if(m_gfx_pointer != nullptr) {
        oamFreeGfx(m_current_oam, m_gfx_pointer);
    }
}

void morpheus::nds::gfx::Sprite::set_sprite_size(const uint8_t width, const uint8_t height) {
    std::cout << "width=" << (int)width << " height=" << (int)height << "\n";

    if(width == height) {
        switch(width) {
            case 8:
                m_sprite_size = SpriteSize_8x8;
                break;
            case 16:
                m_sprite_size = SpriteSize_16x16;
                break;
            case 32:
                m_sprite_size = SpriteSize_32x32;
                break;
            case 64:
                m_sprite_size = SpriteSize_64x64;
                break;
            default:
                // unknown tile size passed
                std::cout << "assert fired\n";
                assert(false);
                break;
        }
    } else if(width == height * 2) {
        switch(width) {
            case 16:
                m_sprite_size = SpriteSize_16x8;
                break;
            case 32:
                m_sprite_size = SpriteSize_32x16;
                break;
            case 64:
                m_sprite_size = SpriteSize_64x32;
                break;
            default:
                // unknown tile size passed
                std::cout << "assert fired\n";
                assert(false);
                break;
        }
    } else if(width * 2 == height) {
        switch(width) {
            case 8:
                m_sprite_size = SpriteSize_8x16;
                break;
            case 16:
                m_sprite_size = SpriteSize_16x32;
                break;
            case 32:
                m_sprite_size = SpriteSize_32x64;
                break;
            default:
                // unknown tile size passed
                std::cout << "assert fired\n";
                assert(false);
                break;
        }
    } else {
        // unknown tile size passed
        std::cout << "assert fired\n";
        assert(false);
    }
}
