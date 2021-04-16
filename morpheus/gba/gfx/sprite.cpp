//
// Created by bobby on 25/07/2020.
//

#include "sprite.hpp"

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
        m_attr1 |= ATTR1_AFF_ID(obj_attr_num);
    }

    obj_set_attr(obj, m_attr0, m_attr1, m_attr2);
    obj_set_pos(obj, position.get_x(), position.get_y());

    if(is_affine()) {
        auto *affine_obj = reinterpret_cast<OBJ_AFFINE *>(obj);

        obj_aff_copy(affine_obj, &m_affine_current, 1);
    }
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

void morpheus::gba::gfx::Sprite::mosaic_state_updated() {
    if(is_mosaic()) {
        m_attr0 |= ATTR0_MOSAIC;
    } else {
        m_attr0 &= ~ATTR0_MOSAIC;
    }
}

void morpheus::gba::gfx::Sprite::toggle_blending(bool enable_blending, bool bottom_layer) {
    get_blending_controller()->disable_object_blending();

    if(enable_blending) {
        m_attr0 |= ATTR0_BLEND;
        m_blended = true;

        get_blending_controller()->enable_object_blending(bottom_layer);
    } else {
        m_attr0 &= ~ATTR0_BLEND;
        m_blended = false;
    }
}

void morpheus::gba::gfx::Sprite::update_affine_state(core::gfx::AffineTransformation affine_transformation,
                                                     bool new_transformation) {
    /*if(new_transformation) {
        obj_aff_identity(&m_affine_new);
    } else {
        obj_aff_copy(&m_affine_base, &m_affine_current, 1);
    }

    switch (affine_transformation) {
        case core::gfx::AffineTransformation::Rotation:
            obj_aff_rotate(&m_affine_new, get_rotation());
            break;
        case core::gfx::AffineTransformation::Scaling:
            obj_aff_scale_inv(&m_affine_new, get_scale().get_x(), get_scale().get_y());
            break;
        case core::gfx::AffineTransformation::Identity:
            obj_aff_identity(&m_affine_current);
            obj_aff_identity(&m_affine_base);
            return;
    }

    if(new_transformation) {
        obj_aff_copy(&m_affine_current, &m_affine_base, 1);
    }

    obj_aff_postmul(&m_affine_current, &m_affine_new);*/

    int cosine_value = lu_cos(get_rotation());
    int sin_value = lu_sin(get_rotation());

    obj_aff_copy(&m_affine_current, &m_affine_base, 1);

    obj_aff_set(&m_affine_current, (cosine_value*get_scale().get_x()) >> 12, (-sin_value*get_scale().get_x()) >> 12,
                (sin_value*get_scale().get_y()) >> 12, (cosine_value*get_scale().get_y()) >> 12);

    //obj_aff_rotate(&m_affine_current, get_rotation());
    //obj_aff_rotscale(&m_affine_current, get_scale().get_x(), get_scale().get_y(),get_rotation());

    std::stringstream nocash_string_stream;

    nocash_string_stream << "Affine matrix:" << std::hex << m_affine_current.pa << ":" << std::hex
                         << m_affine_current.pb << ":" << std::hex << m_affine_current.pc << ":" << std::hex
                         << m_affine_current.pd;

    nocash_puts(nocash_string_stream.str().c_str());
}

