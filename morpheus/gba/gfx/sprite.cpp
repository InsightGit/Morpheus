//
// Created by bobby on 25/07/2020.
//

#include "sprite.hpp"

void morpheus::gba::gfx::Sprite::load_from_array(unsigned short **tile_array, unsigned short tile_array_len,
                                                 unsigned short **pal, unsigned short pal_len,
                                                 unsigned short bit_depth, unsigned short width,
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

    m_bit_depth = bit_depth;
    m_tile_id = tile_id;
    m_tile_array = tile_array;
    m_tile_array_len = tile_array_len;
    m_pal = pal;
    m_pal_len = pal_len;

    std::memcpy(&tile_mem[bit_depth][tile_id], tile_array, tile_array_len);
    std::memcpy(pal_obj_mem, pal, pal_len);
}

void **morpheus::gba::gfx::Sprite::draw(void *obj_attr_buffer[], int obj_attr_num) {

    auto *obj = reinterpret_cast<OBJ_ATTR*>(obj_attr_buffer[obj_attr_num]);

    obj_set_attr(obj, m_attr0, m_attr1, m_attr2);
    obj_set_pos(obj, m_position.get_x(), m_position.get_y());

    //draw_children(obj_attr_buffer, obj_attr_num);

    obj_attr_buffer[obj_attr_num] = obj;

    return obj_attr_buffer;
}

void morpheus::gba::gfx::Sprite::draw_children(void *obj_attr_buffer[], int obj_attr_num) {
    /*if(obj_attr_num + get_children().size() >= 128) {
        // too many sprites!!
        assert(false);
    } else {
        .
    }*/
}
