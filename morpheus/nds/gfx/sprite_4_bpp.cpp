//
// Created by bobby on 16/12/2020.
//

#include "sprite_4_bpp.hpp"

bool morpheus::nds::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                                     const unsigned int width, const unsigned int height) {

    set_sprite_size(width, height);

    allocate_gfx_pointer(SpriteColorFormat_16Color);

    //std::cout << "loading 4bpp tiled array to palette #" << palette_id << "\n";

    for(unsigned int i = 0; i < (width * height) / 2u; ++i) {
        get_gfx_pointer()[i] = *(tile_array + i);
    }

    set_palette_id(palette_id);

    //std::cout << "loaded tiled array to palette #" << palette_id << "\n";

    return true;
}

bool morpheus::nds::gfx::Sprite4Bpp::load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                                     const unsigned short *palette, const unsigned int width,
                                                     const unsigned int height) {
    if(!load_from_array(tile_array, palette_id, width, height)) {
        return false;
    }

    load_into_palette(palette, palette_id);

    return true;
}

bool morpheus::nds::gfx::Sprite4Bpp::load_into_palette(const unsigned short *palette, const unsigned int palette_id,
                                                       const unsigned int pal_len) {
    unsigned int palette_index;

    if(palette_id > 15) {
        sassert(false, "Palette number in 4bpp sprite above 16");
        return false;
    } else if(palette_id == 0) {
        palette_index = 0;
    } else {
        palette_index = palette_id * 16;
    }

    if(get_current_oam() == &oamSub) {
        dmaCopy(&palette[palette_index], SPRITE_PALETTE_SUB, pal_len);
    } else {
        dmaCopy(&palette[palette_index], SPRITE_PALETTE, pal_len);
    }

    return true;
}

void morpheus::nds::gfx::Sprite4Bpp::draw_node(std::vector<void *>& obj_attr_buffer, int obj_attr_num, int priority) {
    core::gfx::Vector2 position = get_position();

    set_last_used_obj_attr_num(obj_attr_num);

    oamSet(get_current_oam(), obj_attr_num, position.get_x(), position.get_y(), priority,
           static_cast<int>(get_palette_id()), get_sprite_size(),
           SpriteColorFormat_16Color, get_gfx_pointer(), -1,
           false, false, false, false, false);
}
