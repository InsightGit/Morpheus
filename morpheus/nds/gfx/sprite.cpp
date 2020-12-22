//
// Created by bobby on 17/12/2020.
//

#include "sprite.hpp"

morpheus::nds::gfx::Sprite::Sprite(const bool use_sub_display, const SpriteMapping sprite_mapping,
                                   ExtendedPaletteStatus extended_palette) {
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

    // TODO(Bobby): Investigate bitmap spritemapping preservation while non-bitmap sprite is loaded
    switch(extended_palette) {
        case ExtendedPaletteStatus::NOTNEEDED:
            m_extended_palette = (OAM_STATUS == OamStatus::ENABLED_EXTENDED ||
                                  OAM_STATUS == OamStatus::ENABLED_EXTENDED_BITMAP);
            break;
        case ExtendedPaletteStatus::NEEDED:
            m_extended_palette = true;
            break;
        case ExtendedPaletteStatus::NEEDOFF:
            m_extended_palette = false;
            break;
    }

    if(m_extended_palette) {
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

        oamInit(m_current_oam, sprite_mapping, m_extended_palette);
    }
}

morpheus::nds::gfx::Sprite::~Sprite() {
    if(m_gfx_pointer != nullptr) {
        oamFreeGfx(m_current_oam, m_gfx_pointer);
    }
}

void morpheus::nds::gfx::Sprite::allocate_gfx_pointer(SpriteColorFormat color_format, uint8_t width, uint8_t height) {
    if(width > 0 && height > 0) {
        set_sprite_size(width, height);
    }

    if(m_gfx_pointer != nullptr) {
        std::cout << "freeing gfx\n";

        oamFreeGfx(m_current_oam, m_gfx_pointer);
    }

    m_gfx_pointer = oamAllocateGfx(m_current_oam, m_sprite_size, color_format);
}

void morpheus::nds::gfx::Sprite::set_sprite_size(const uint8_t width, const uint8_t height) {
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
