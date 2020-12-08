//
// Created by bobby on 27/11/2020.
//

#include "sprite.hpp"

morpheus::nds::gfx::Sprite::Sprite(bool use_sub_display) {
    if(use_sub_display) {
        m_current_oam = &oamSub;

        vramSetBankD(VRAM_D_SUB_SPRITE);

    } else {
        m_current_oam = &oamMain;

        vramSetBankB(VRAM_B_MAIN_SPRITE);
    }

    oamInit(m_current_oam, SpriteMapping_1D_32, false);
}

void morpheus::nds::gfx::Sprite::set_sprite_size(uint8_t width, uint8_t height) {
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
                assert(false);
                break;
        }
    } else {
        // unknown tile size passed
        assert(false);
    }
}

void morpheus::nds::gfx::Sprite::load_from_array(uint8_t **tile_array, uint8_t tile_array_len,
                                                 uint8_t **pal, uint8_t pal_len, uint8_t width,
                                                 uint8_t height, uint8_t tile_id) {
    set_sprite_size(width, height);
}

bool morpheus::nds::gfx::Sprite::load_from_pcx(const unsigned char *pcx_data, uint8_t palette_id,
                                               bool copy_palette) {
    bool return_value = loadPCX(pcx_data, m_sprite_image.get()) == 0;

    if(return_value) {
        imageTileData(m_sprite_image.get());
    }

    set_sprite_size(m_sprite_image->width, m_sprite_image->height);

    if(m_current_oam == &oamSub) {
        vramSetBankI(VRAM_I_LCD);
    } else {
        vramSetBankF(VRAM_F_LCD);
    }

    if(copy_palette) {
        for(int i = 0; i < 256; ++i) {
            VRAM_F_EXT_SPR_PALETTE[palette_id][i] = m_sprite_image->palette[i];
        }
    }

    m_gfx_pointer = oamAllocateGfx(m_current_oam, m_sprite_size, SpriteColorFormat_256Color);

    for(int i = 0; i < m_sprite_image->width * m_sprite_image->height; ++i) {
        m_gfx_pointer[i] = m_sprite_image->image.data16[i];
    }

    if(m_current_oam == &oamSub) {
        vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
    } else {
        vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
    }

    return return_value;
}

void morpheus::nds::gfx::Sprite::draw(std::vector<void *>obj_attr_buffer, int obj_attr_num) {
    oamSet(m_current_oam, obj_attr_num, m_position.get_x(), m_position.get_y(), 0, m_palette_id, m_sprite_size,
           SpriteColorFormat_256Color, m_gfx_pointer, -1, false, false, false, false, false);
}
