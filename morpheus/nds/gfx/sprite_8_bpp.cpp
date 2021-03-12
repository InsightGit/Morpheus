//
// Created by bobby on 27/11/2020.
//

#include "sprite_8_bpp.hpp"

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array, const unsigned int width,
                                                     const unsigned int height) {
    //std::cout << "loading tiled array in single palette mode\n";

    if(is_in_extended_palette_mode()) {
        std::cout << "ERROR: the DS is not in single palette mode!\n";
        return false;
    }

    set_sprite_size(width, height);

    allocate_gfx_pointer(SpriteColorFormat_256Color);

    dmaCopy(tile_array, get_gfx_pointer(), width * height);

    //std::cout << "loaded tiled array in single palette mode\n";

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                                     const unsigned int palette_len, const unsigned int width,
                                                     const unsigned int height) {
    if(!load_from_array(tile_array, width, height)) {
        return false;
    }

    load_into_palette(palette, palette_len);

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                                     const unsigned int width, const unsigned int height) {
    if(!is_in_extended_palette_mode()) {
        std::cout << "ERROR: the DS is not in extended palette mode!\n";
        return false;
    }

    set_sprite_size(width, height);

    allocate_gfx_pointer(SpriteColorFormat_256Color);

    //std::cout << "loading tiled array in extended palette mode\n";

    dmaCopy(tile_array, get_gfx_pointer(),width * height);

    set_palette_id(palette_id);

    ///std::cout << "loaded tiled array in extended palette mode\n";

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                                     const unsigned int palette_len, const unsigned int palette_id,
                                                     const unsigned int width, const unsigned int height) {
    if(!load_from_array(tile_array, palette_id, width, height)) {
        return false;
    }

    load_into_palette(m_sprite_image->palette, palette_len);

    return true;
}

bool morpheus::nds::gfx::Sprite8Bpp::load_from_pcx(const unsigned char *pcx_data, unsigned int palette_id,
                                                   bool copy_palette) {
    bool return_value = loadPCX(pcx_data, m_sprite_image.get()) == 1;

    //std::cout << "loading pcx\n";

    if(return_value) {
        imageTileData(m_sprite_image.get());
    } else {
        return false;
    }

    set_sprite_size(m_sprite_image->width, m_sprite_image->height);

    //std::cout << "allocating gfx\n";

    allocate_gfx_pointer(SpriteColorFormat_256Color);

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

bool morpheus::nds::gfx::Sprite8Bpp::load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                                       const unsigned int pal_len) {
    OamState *current_oam = get_current_oam();
    std::string oam_name;

    if(is_in_extended_palette_mode()) {
        if(current_oam == &oamSub) {
            oam_name = "oamSub";

            vramSetBankI(VRAM_I_LCD);

            dmaCopy(palette, &VRAM_I_EXT_SPR_PALETTE[palette_id], pal_len);
        } else {
            oam_name = "oamMain";

            vramSetBankF(VRAM_F_LCD);

            dmaCopy(palette, &VRAM_F_EXT_SPR_PALETTE[palette_id], pal_len);
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
            dmaCopy(palette, &SPRITE_PALETTE_SUB[0], pal_len);
        } else {
            dmaCopy(palette, &SPRITE_PALETTE[0], pal_len);
        }

        //std::cout << "loaded 8bpp palette (single palette mode)\n";
    }

    return true;
}

void morpheus::nds::gfx::Sprite8Bpp::draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) {
    core::gfx::Vector2 position = get_position();

    set_last_used_obj_attr_num(obj_attr_num);

    oamSet(get_current_oam(), obj_attr_num, position.get_x(), position.get_y(), priority,
           static_cast<int>(get_palette_id()),get_sprite_size(), SpriteColorFormat_256Color,
           get_gfx_pointer(), -1, false, false, false, false, false);
}
