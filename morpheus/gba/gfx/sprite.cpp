//
// Created by bobby on 25/07/2020.
//

#include "sprite.hpp"

void morpheus::gba::gfx::Sprite::draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) {
    auto *obj = static_cast<OBJ_ATTR *>(obj_attr_buffer[obj_attr_num]);
    core::gfx::Vector2 position = get_position();

    m_attr2 |= ATTR2_PRIO(m_priority);

    if(m_is_4bpp) {
        m_attr0 |= ATTR0_4BPP;
    } else {
        m_attr0 |= ATTR0_8BPP;
    }

    m_last_obj_attr_num = obj_attr_num;

    obj_set_attr(obj, m_attr0, m_attr1, m_attr2);
    obj_set_pos(obj, position.get_x(), position.get_y());
}

void morpheus::gba::gfx::Sprite::set_sprite_size(core::gfx::SpriteSize size) {
    switch(size) {
        case core::gfx::SpriteSize::SIZE_8X8:
        case core::gfx::SpriteSize::SIZE_16X16:
        case core::gfx::SpriteSize::SIZE_32X32:
        case core::gfx::SpriteSize::SIZE_64X64:
            m_attr0 = ATTR0_SQUARE;
            break;
        case core::gfx::SpriteSize::SIZE_16X8:
        case core::gfx::SpriteSize::SIZE_32X16:
        case core::gfx::SpriteSize::SIZE_64X32:
            m_attr0 = ATTR0_WIDE;
            break;
        case core::gfx::SpriteSize::SIZE_8X16:
        case core::gfx::SpriteSize::SIZE_16X32:
        case core::gfx::SpriteSize::SIZE_32X64:
            m_attr0 = ATTR0_TALL;
            break;
    }

    switch(size) {
        case core::gfx::SpriteSize::SIZE_8X8:
            m_attr1 = ATTR1_SIZE_8;
            break;
        case core::gfx::SpriteSize::SIZE_8X16:
        case core::gfx::SpriteSize::SIZE_16X8:
        case core::gfx::SpriteSize::SIZE_16X16:
            m_attr1 = ATTR1_SIZE_16;
            break;
        case core::gfx::SpriteSize::SIZE_16X32:
        case core::gfx::SpriteSize::SIZE_32X16:
        case core::gfx::SpriteSize::SIZE_32X32:
            m_attr1 = ATTR1_SIZE_32;
            break;
        case core::gfx::SpriteSize::SIZE_32X64:
        case core::gfx::SpriteSize::SIZE_64X32:
        case core::gfx::SpriteSize::SIZE_64X64:
            m_attr1 = ATTR1_SIZE_64;
            break;
    }
}

void morpheus::gba::gfx::Sprite::load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                                 const unsigned short *palette, const unsigned int palette_len,
                                                 const core::gfx::SpriteSize size, const unsigned int tile_id) {
    /*m_attr0 = 0x0;
    m_attr1 = 0x0;
    m_attr2 = 0x0;*/

    set_sprite_size(size);

    array_load(tile_array, tile_array_len, palette, palette_len, size, tile_id);
}

void morpheus::gba::gfx::Sprite::load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                                 const core::gfx::SpriteSize size, const unsigned int tile_id) {
    m_attr0 = 0x0;
    m_attr1 = 0x0;
    m_attr2 = 0x0;

    set_sprite_size(size);

    array_load(tile_array, tile_array_len, size, tile_id);
}

void morpheus::gba::gfx::Sprite::on_visible_state_changed(bool hidden) {
    if(hidden) {
        m_attr0 |= ATTR0_HIDE;
    } else {
        m_attr0 |= ATTR0_REG;
    }
}

morpheus::gba::gfx::Sprite::~Sprite() {
    if(!is_hidden()) {
        OBJ_ATTR obj_attr;

        obj_attr.attr0 = ATTR0_HIDE;

        oam_copy(oam_mem + (morpheus::gba::GbaMainLoop::OBJ_ATTR_SIZE * m_last_obj_attr_num),
                 static_cast<OBJ_ATTR *>(&obj_attr), 1);

        nocash_puts("cleared obj attr");
    }
}
