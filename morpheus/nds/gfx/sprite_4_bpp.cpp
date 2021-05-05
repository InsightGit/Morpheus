//
// Created by bobby on 16/12/2020.
//

#include "sprite_4_bpp.hpp"

bool morpheus::nds::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len,
                                                     const unsigned int palette_id,
                                                     core::gfx::SpriteSize size) {

    set_sprite_size(size);

    set_gfx_pointer(create_gfx_pointer(SpriteColorFormat_16Color, size));

    //std::cout << "loading 4bpp tiled array to palette #" << palette_id << "\n";

    dmaCopy(tile_array, get_gfx_pointer(), tile_array_len);

    set_palette_id(palette_id);

    //std::cout << "loaded tiled array to palette #" << palette_id << "\n";

    return true;
}

bool morpheus::nds::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array,
                                                     const unsigned int tile_array_len, const unsigned short *palette,
                                                     const unsigned int palette_len, const unsigned int palette_id,
                                                     core::gfx::SpriteSize size) {
    if(!load_from_array(tile_array, tile_array_len, palette_id, size)) {
        return false;
    }

    load_into_palette(palette, palette_len);

    return true;
}

bool morpheus::nds::gfx::Sprite4Bpp::load_into_palette(const unsigned short *palette, const unsigned int palette_len) {

    if(get_current_oam() == &oamSub) {
        dmaCopy(&palette[0], SPRITE_PALETTE_SUB, palette_len);
    } else {
        dmaCopy(&palette[0], SPRITE_PALETTE, palette_len);
    }

    return true;
}

void morpheus::nds::gfx::Sprite4Bpp::draw_node(std::vector<void *>& obj_attr_buffer, unsigned int obj_attr_num) {
    core::gfx::Vector2 position = get_position();

    set_last_used_obj_attr_num(static_cast<int>(obj_attr_num));

    if(is_affine()) {
        oamRotateScale(get_current_oam(), static_cast<int>(get_affine_index()), get_rotation(),
                       get_scale().get_x(), get_scale().get_y());
    }

    oamSet(get_current_oam(), static_cast<int>(obj_attr_num), position.get_x(), position.get_y(),
           static_cast<int>(get_priority()), static_cast<int>(get_palette_id()),
           get_nds_sprite_size(), SpriteColorFormat_16Color, get_gfx_pointer(), static_cast<int>(get_affine_index()),
           is_affine(), false, false, false, is_mosaic());

    if(is_blended()) {
        get_current_oam()->oamMemory[obj_attr_num].blendMode = OBJMODE_BLENDED;
    } else {
        get_current_oam()->oamMemory[obj_attr_num].blendMode = OBJMODE_NORMAL;
    }
}
