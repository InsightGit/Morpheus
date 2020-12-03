//
// Created by bobby on 25/07/2020.
//

#include "sprite.hpp"

void morpheus::gba::gfx::Sprite::load_from_array(unsigned short **tile_array, unsigned short tile_array_len,
                                                 unsigned short **pal, unsigned short pal_len,
                                                 unsigned short width,
                                                 unsigned short height, unsigned short tile_id) {
    m_attr0 = 0x0;
    m_attr1 = 0x0;

    if(width == height && (width == 8 || width == 16 || width == 32 || width == 64)) {
        m_attr0 = ATTR0_SQUARE;
    } else if(width == (height * 2) && (width == 16 || width == 32 || width == 64)) {
        m_attr0 = ATTR0_WIDE;
    } else if((width * 2) == height && (width == 8 || width == 16 || width == 32)) {
        m_attr0 = ATTR0_TALL;
    } else {
        // unknown tile size passed
        assert(false);
    }

    switch(max(width, height)) {
        case 8:
            m_attr1 = ATTR1_SIZE_8;
            break;
        case 16:
            m_attr1 = ATTR1_SIZE_16;
            break;
        case 32:
            m_attr1 = ATTR1_SIZE_32;
            break;
        case 64:
            m_attr1 = ATTR1_SIZE_64;
            break;
        default:
            // unknown tile size passed
            assert(false);
    }

    m_attr2 = ATTR2_BUILD(tile_id, 0, 0);

    // For objects on the GBA, the bit depth is always 4
    std::memcpy(&tile_mem[4][tile_id], tile_array, tile_array_len);
    std::memcpy(pal_obj_mem, pal, pal_len);
}

void morpheus::gba::gfx::Sprite::draw(OBJ_ATTR (*obj_attr_buffer)[], int obj_attr_num) {
    OBJ_ATTR *obj = &((*obj_attr_buffer)[obj_attr_num]);

    obj_set_attr(obj, m_attr0, m_attr1, m_attr2);
    obj_set_pos(obj, m_position.get_x(), m_position.get_y());
}

void morpheus::gba::gfx::Sprite::draw_children(OBJ_ATTR (*obj_attr_buffer)[], int obj_attr_num) {
    /*if(obj_attr_num + get_children().size() >= 128) {
        // too many sprites!!
        assert(false);
    } else {
        .
    }*/
}
