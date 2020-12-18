//
// Created by bobby on 27/11/2020.
//

#include "sprite_8_bpp.hpp"

void morpheus::nds::gfx::Sprite8Bpp::copy_into_palette(uint16_t **palette, uint8_t palette_id) {
    OamState *current_oam = get_current_oam();
    std::string oam_name;

    if(is_in_extended_palette_mode()) {
        if(current_oam == &oamSub) {
            oam_name = "oamSub";

            vramSetBankI(VRAM_I_LCD);
        } else {
            oam_name = "oamMain";

            vramSetBankF(VRAM_F_LCD);
        }

        m_palette_id = palette_id;

        for(int i = 0; i < 256; ++i) {
            if(current_oam == &oamSub) {
                VRAM_I_EXT_SPR_PALETTE[m_palette_id][i] = (*palette)[i];
            } else {
                VRAM_F_EXT_SPR_PALETTE[m_palette_id][i] = (*palette)[i];
            }
        }

        std::cout << "loaded 8bpp palette into " << oam_name << "\n" << "Palette #" << int(m_palette_id) << "\n";

        if(current_oam == &oamSub) {
            vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
        } else {
            vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
        }
    } else {
        for(int i = 0; i < 256; ++i) {
            if(current_oam == &oamSub) {
                SPRITE_PALETTE_SUB[i] = (*palette)[i];
            } else {
                SPRITE_PALETTE[i] = (*palette)[i];
            }
        }

        std::cout << "loaded 8bpp palette (single palette mode)\n";
    }
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(uint8_t **tile_array, uint8_t width, uint8_t height) {
    std::cout << "loading tiled array in single palette mode\n";

    if(is_in_extended_palette_mode()) {
        std::cout << "ERROR: this sprite is not in single palette mode!\n";
        return false;
    }

    set_sprite_size(width, height);

    allocate_gfx_pointer(SpriteColorFormat_256Color);

    for(int i = 0; i < (width * height) / 2; ++i) {
        get_gfx_pointer()[i] = (*tile_array)[i];
    }

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(uint8_t **tile_array, uint16_t **palette, uint8_t width,
                                                     uint8_t height) {
    std::cout << "loading tiled array in single palette mode\n";

    if(!is_in_extended_palette_mode()) {
        std::cout << "ERROR: this sprite is not extended palette mode!\n";
        return false;
    }


    std::cout << "loaded tiled array in single palette mode\n";

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(uint8_t **tile_array, uint8_t palette_id, uint8_t width,
                                                     uint8_t height) {
    std::cout << "loading tiled array in extended palette mode\n";

    if(!is_in_extended_palette_mode()) {
        std::cout << "ERROR: this sprite is not extended palette mode!\n";
        return false;
    }

    set_sprite_size(width, height);

    allocate_gfx_pointer(SpriteColorFormat_256Color);

    for(int i = 0; i < (width * height) / 2; ++i) {
        get_gfx_pointer()[i] = (*tile_array)[i];
    }

    m_palette_id = palette_id;

    std::cout << "loaded tiled array in extended palette mode\n";

    return true;
}

// TODO(Bobby): Investigate getting rid of unused tile_array_len, pal_len
bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(uint8_t **tile_array, uint8_t palette_id,
                                                     uint16_t **palette, uint8_t width, uint8_t height) {
    if(!load_from_array(tile_array, palette_id, width, height)) {
        return false;
    }

    copy_into_palette(palette, palette_id);

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_pcx(const unsigned char *pcx_data, const uint8_t palette_id,
                                                   const bool copy_palette) {
    bool return_value = loadPCX(pcx_data, m_sprite_image.get()) == 1;

    if(return_value) {
        imageTileData(m_sprite_image.get());
    } else {
        return false;
    }

    set_sprite_size(m_sprite_image->width, m_sprite_image->height);

    allocate_gfx_pointer(SpriteColorFormat_256Color);

    for(int i = 0; i < (m_sprite_image->width * m_sprite_image->height) / 2; ++i) {
        get_gfx_pointer()[i] = m_sprite_image->image.data16[i];
    }

    if(copy_palette) {
        copy_into_palette(&m_sprite_image->palette, palette_id);
    }

    std::cout << "loaded pcx\n";

    return true;
}

void morpheus::nds::gfx::Sprite8Bpp::draw(std::vector<void *>obj_attr_buffer, int obj_attr_num) {
    core::gfx::Vector2 position = get_position();

    if(obj_attr_num != 0) {
        std::cout << "oh noes: " << obj_attr_num << "\n";
    }

    oamSet(get_current_oam(), obj_attr_num, position.get_x(), position.get_y(), 0, m_palette_id,
           get_sprite_size(), SpriteColorFormat_256Color, get_gfx_pointer(), -1, false, false, false, false, false);
}
