//
// Created by bobby on 25/07/2020.
//

#include "sprite.hpp"

void morpheus::gba::gfx::Sprite::draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) {
    auto *obj = static_cast<OBJ_ATTR *>(obj_attr_buffer[obj_attr_num]);

    if(priority < 3 && priority >= 0) {
        m_attr2 |= ATTR2_PRIO(priority);
    } else {
        m_attr2 |= ATTR2_PRIO(3);
    }

    if(m_is_4bpp) {
        m_attr0 |= ATTR0_4BPP;
    } else {
        m_attr0 |= ATTR0_8BPP;
    }

    obj_set_attr(obj, m_attr0, m_attr1, m_attr2);
    obj_set_pos(obj, m_position.get_x(), m_position.get_y());
}

void morpheus::gba::gfx::Sprite::setup_size_attr(const unsigned short width, const unsigned short height) {
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
}

void morpheus::gba::gfx::Sprite::load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                                 const unsigned short width, const unsigned short height,
                                                 const unsigned short tile_id) {
    m_attr0 = 0x0;
    m_attr1 = 0x0;
    m_attr2 = 0x0;

    setup_size_attr(width, height);

    array_load(tile_array, palette, width, height, tile_id);
}

void morpheus::gba::gfx::Sprite::load_from_array(const unsigned short *tile_array, const unsigned short width,
                                                 const unsigned short height, const unsigned short tile_id) {
    m_attr0 = 0x0;
    m_attr1 = 0x0;
    m_attr2 = 0x0;

    setup_size_attr(width, height);

    array_load(tile_array, width, height, tile_id);
}
