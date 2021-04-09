//
// Created by bobby on 07/04/2021.
//

#include "affine_sprite_8_bpp.hpp"

void morpheus::gba::gfx::AffineSprite8Bpp::draw_node(std::vector<void *> &obj_attr_buffer,
                                                     unsigned int obj_attr_num) {
    OBJ_AFFINE *obj;

    Sprite::draw_node(obj_attr_buffer, obj_attr_num);

    obj = static_cast<OBJ_AFFINE *>(obj_attr_buffer[obj_attr_num]);

    obj_set_attr(reinterpret_cast<OBJ_ATTR*>(obj), get_attr0() | ATTR0_AFF,
                 get_attr1() | ATTR1_AFF_ID(obj_attr_num), get_attr2());

    obj_aff_identity(obj);
    obj_aff_rotscale(obj, m_scale.get_x(), m_scale.get_y(), m_rotation);
}
