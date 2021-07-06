//
// Created by bobby on 25/07/2020.
//

#include "morpheus/gba/gfx/sprite.hpp"

void morpheus::gba::gfx::Sprite::draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    auto *obj = static_cast<OBJ_ATTR *>(obj_attr_buffer[obj_attr_num]);
    core::gfx::Vector2 position = get_position();

    m_attr2 |= ATTR2_PRIO(get_priority());

    if(m_is_4bpp) {
        m_attr0 |= ATTR0_4BPP;
    } else {
        m_attr0 |= ATTR0_8BPP;
    }

    m_last_obj_attr_num = obj_attr_num;

    if(is_affine()) {
        m_attr0 |= ATTR0_AFF | ATTR0_AFF_DBL;
        m_attr1 |= ATTR1_AFF_ID(get_affine_index());
    } else if(is_blended()) {
        m_attr0 |= ATTR0_BLEND;
    }

    if(is_mosaic()) {
        m_attr0 |= ATTR0_MOSAIC;
    }

    obj_set_attr(obj, m_attr0, m_attr1, m_attr2);
    obj_set_pos(obj, position.get_x(), position.get_y());

    obj->fill = m_affine_current.pa;

    if(is_affine()) {
        obj_aff_copy(&obj_aff_mem[get_affine_index()], &m_affine_current, 1);
    }
}

void morpheus::gba::gfx::Sprite::set_sprite_size(core::gfx::SpriteSize size) {
    m_sprite_size = size;

    m_attr0 &= ~ATTR0_SHAPE_MASK;
    m_attr1 &= ~ATTR1_SIZE_MASK;

    switch(m_sprite_size) {
        case core::gfx::SpriteSize::SIZE_8X8:
        case core::gfx::SpriteSize::SIZE_16X16:
        case core::gfx::SpriteSize::SIZE_32X32:
        case core::gfx::SpriteSize::SIZE_64X64:
            m_attr0 |= ATTR0_SQUARE;
            break;
        case core::gfx::SpriteSize::SIZE_16X8:
        case core::gfx::SpriteSize::SIZE_32X16:
        case core::gfx::SpriteSize::SIZE_64X32:
            m_attr0 |= ATTR0_WIDE;
            break;
        case core::gfx::SpriteSize::SIZE_8X16:
        case core::gfx::SpriteSize::SIZE_16X32:
        case core::gfx::SpriteSize::SIZE_32X64:
            m_attr0 |= ATTR0_TALL;
            break;
    }

    switch(m_sprite_size) {
        case core::gfx::SpriteSize::SIZE_8X8:
            m_attr1 |= ATTR1_SIZE_8;
            break;
        case core::gfx::SpriteSize::SIZE_8X16:
        case core::gfx::SpriteSize::SIZE_16X8:
        case core::gfx::SpriteSize::SIZE_16X16:
            m_attr1 |= ATTR1_SIZE_16;
            break;
        case core::gfx::SpriteSize::SIZE_16X32:
        case core::gfx::SpriteSize::SIZE_32X16:
        case core::gfx::SpriteSize::SIZE_32X32:
            m_attr1 |= ATTR1_SIZE_32;
            break;
        case core::gfx::SpriteSize::SIZE_32X64:
        case core::gfx::SpriteSize::SIZE_64X32:
        case core::gfx::SpriteSize::SIZE_64X64:
            m_attr1 |= ATTR1_SIZE_64;
            break;
    }
}

void morpheus::gba::gfx::Sprite::load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                                 const unsigned short *palette, const unsigned int palette_len,
                                                 const core::gfx::SpriteSize size, const unsigned int tile_id) {
    m_attr0 = 0x0;
    m_attr1 = 0x0;
    m_attr2 = 0x0;

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
    m_attr0 &= ~ATTR0_MODE_MASK;

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

    nocash_puts("deconstructed sprite");
}

void morpheus::gba::gfx::Sprite::mosaic_state_updated() {
    if(is_mosaic()) {
        m_mosaic = true;
    } else {
        m_attr0 &= ~ATTR0_MOSAIC;

        m_mosaic = false;
    }
}

void morpheus::gba::gfx::Sprite::toggle_blending(bool enable_blending, bool bottom_layer) {
    get_blending_controller()->disable_object_blending();

    m_attr0 &= 0xF3FF;

    if(enable_blending) {
        nocash_puts("blending enabled");

        m_blended = true;

        get_blending_controller()->enable_object_blending(bottom_layer);
    } else {
        nocash_puts("blending disabled");

        m_attr0 |= ATTR0_REG;
        m_blended = false;
    }


}

void morpheus::gba::gfx::Sprite::update_affine_state(core::gfx::AffineTransformation affine_transformation,
                                                     bool new_transformation) {
    OBJ_AFFINE affine_new;

    obj_aff_identity(&affine_new);

    if(new_transformation) {
        obj_aff_copy(&m_affine_base, &m_affine_current, 1);
    } else {
        obj_aff_copy(&m_affine_current, &m_affine_base, 1);
    }

    switch(affine_transformation) {
        case core::gfx::AffineTransformation::Rotation:
            obj_aff_rotate_inv(&m_affine_current, get_rotation());
            break;
        case core::gfx::AffineTransformation::Scaling:
            obj_aff_scale_inv(&m_affine_current, get_scale().get_x(), get_scale().get_y());
            break;
        case core::gfx::AffineTransformation::Identity:
            obj_aff_identity(&m_affine_base);
            obj_aff_identity(&m_affine_current);

            break;
    };

    obj_aff_postmul(&m_affine_current, &affine_new);
}
