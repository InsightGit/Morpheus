//
// Created by bobby on 27/11/2020.
//

#include "sprite_8_bpp.hpp"

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len,
                                                     const morpheus::core::gfx::SpriteSize size) {
    //std::cout << "loading tiled array in single palette mode\n";

    if(is_in_extended_palette_mode()) {
        std::cout << "ERROR: the DS is not in single palette mode!\n";
        return false;
    }

    set_sprite_size(size);

    set_gfx_pointer(create_gfx_pointer(SpriteColorFormat_256Color, size));

    dmaCopy(tile_array, get_gfx_pointer(), tile_array_len);

    //std::cout << "loaded tiled array in single palette mode\n";

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len, const unsigned short *palette,
                                                     const unsigned int palette_len,
                                                     const morpheus::core::gfx::SpriteSize size) {
    if(!load_from_array(tile_array, tile_array_len, size)) {
        return false;
    }

    load_into_palette(palette, palette_len);

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len, const unsigned int palette_id,
                                                     const morpheus::core::gfx::SpriteSize size) {
    if(!is_in_extended_palette_mode()) {
        std::cout << "ERROR: the DS is not in extended palette mode!\n";
        return false;
    }

    set_sprite_size(size);

    set_gfx_pointer(create_gfx_pointer(SpriteColorFormat_256Color, size));

    //std::cout << "loading tiled array in extended palette mode\n";

    dmaCopy(tile_array, get_gfx_pointer(), tile_array_len);

    set_palette_id(palette_id);

    ///std::cout << "loaded tiled array in extended palette mode\n";

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len, const unsigned short *palette,
                                                     const unsigned int palette_len, const unsigned int palette_id,
                                                     const morpheus::core::gfx::SpriteSize size) {
    if(!load_from_array(tile_array, tile_array_len, palette_id, size)) {
        return false;
    }

    load_into_palette(m_sprite_image->palette, palette_len);

    nocashMessage("Loaded into palette");

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_pcx(const unsigned char *pcx_data, unsigned int palette_id,
                                                   bool copy_palette) {
    morpheus::core::gfx::SpriteSize sprite_size;
    bool return_value = loadPCX(pcx_data, m_sprite_image.get()) == 1;

    //std::cout << "loading pcx\n";

    if(return_value) {
        imageTileData(m_sprite_image.get());
    } else {
        return false;
    }

    if(m_sprite_image->width == m_sprite_image->height) {
        switch(m_sprite_image->width) {
            case 8:
                sprite_size = core::gfx::SpriteSize::SIZE_8X8;
                break;
            case 16:
                sprite_size = core::gfx::SpriteSize::SIZE_16X16;
                break;
            case 32:
                sprite_size = core::gfx::SpriteSize::SIZE_32X32;
                break;
            case 64:
                sprite_size = core::gfx::SpriteSize::SIZE_64X64;
                break;
            default:
                return false;
        }
    } else if(m_sprite_image->width == 2 * m_sprite_image->height) {
        switch(m_sprite_image->width) {
            case 8:
                sprite_size = core::gfx::SpriteSize::SIZE_8X16;
                break;
            case 16:
                sprite_size = core::gfx::SpriteSize::SIZE_16X32;
                break;
            case 32:
                sprite_size = core::gfx::SpriteSize::SIZE_32X64;
                break;
            default:
                return false;
        }
    } else if(2 * m_sprite_image->width == m_sprite_image->height) {
        switch(m_sprite_image->width) {
            case 16:
                sprite_size = core::gfx::SpriteSize::SIZE_16X8;
                break;
            case 32:
                sprite_size = core::gfx::SpriteSize::SIZE_32X16;
                break;
            case 64:
                sprite_size = core::gfx::SpriteSize::SIZE_64X32;
                break;
            default:
                return false;
        }
    } else {
        return false;
    }

    set_sprite_size(sprite_size);

    //std::cout << "allocating gfx\n";

    set_gfx_pointer(create_gfx_pointer(SpriteColorFormat_256Color, sprite_size));

    //std::cout << "copying into gfx\n";

    dmaCopy(m_sprite_image->image.data16, get_gfx_pointer(),
            (m_sprite_image->width * m_sprite_image->height));

    if(copy_palette) {
        //std::cout << "copying into palette\n";

        load_into_palette(m_sprite_image->palette, palette_id);
    }

    //std::cout << "loaded pcx\n";

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_into_palette(const unsigned short *palette, const unsigned int palette_len,
                                                       const unsigned int pal_offset,
                                                       const unsigned int palette_id) {
    OamState *current_oam = get_current_oam();
    std::string oam_name;

    if(is_in_extended_palette_mode()) {
        if(current_oam == &oamSub) {
            oam_name = "oamSub";

            vramSetBankI(VRAM_I_LCD);

            if(pal_offset < 256) {
                dmaCopy(palette, &VRAM_I_EXT_SPR_PALETTE[palette_id] + pal_offset, palette_len);
            } else {
                dmaCopy(palette, &VRAM_I_EXT_SPR_PALETTE[palette_id], palette_len);
            }
        } else {
            oam_name = "oamMain";

            vramSetBankF(VRAM_F_LCD);

            if(pal_offset < 256) {
                dmaCopy(palette, &VRAM_F_EXT_SPR_PALETTE[palette_id] + pal_offset, palette_len);
            } else {
                dmaCopy(palette, &VRAM_F_EXT_SPR_PALETTE[palette_id], palette_len);
            }
        }

        set_palette_id(palette_id);

        //std::cout << "loaded 8bpp palette into " << oam_name << "\n" << "Palette #" << int(palette_id) << "\n";

        if(current_oam == &oamSub) {
            vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
        } else {
            vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
        }
    } else {
        if(current_oam == &oamSub) {
            if(pal_offset < 256) {
                dmaCopy(palette, &SPRITE_PALETTE_SUB[0] + pal_offset, palette_len);
            } else {
                dmaCopy(palette, &SPRITE_PALETTE_SUB[0], palette_len);
            }
        } else {
            if(pal_offset < 256) {
                dmaCopy(palette, &SPRITE_PALETTE[0] + pal_offset, palette_len);
            } else {
                dmaCopy(palette, &SPRITE_PALETTE[0], palette_len);
            }
        }

        //nocashMessage("loaded 8bpp palette (single palette mode)\n");
        //nocashMessage(std::string("palette len:" + std::to_string(palette_len)).c_str());
    }

    return true;
}

void morpheus::nds::gfx::Sprite8Bpp::draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    core::gfx::Vector2 position = get_position();

    set_last_used_obj_attr_num(static_cast<int>(obj_attr_num));

    if(is_affine()) {
        oamRotateScale(get_current_oam(), static_cast<int>(get_affine_index()), get_rotation(),
                       get_scale().get_x(), get_scale().get_y());
    }

    oamSet(get_current_oam(), static_cast<int>(obj_attr_num), position.get_x(), position.get_y(),
           static_cast<int>(get_priority()),static_cast<int>(get_palette_id()), get_nds_sprite_size(),
           SpriteColorFormat_256Color,get_gfx_pointer(), static_cast<int>(get_affine_index()), is_affine(), is_hidden(),
           false, false, is_mosaic());

    if(is_blended()) {
        get_current_oam()->oamMemory[obj_attr_num].blendMode = OBJMODE_BLENDED;
    } else {
        get_current_oam()->oamMemory[obj_attr_num].blendMode = OBJMODE_NORMAL;
    }
}
