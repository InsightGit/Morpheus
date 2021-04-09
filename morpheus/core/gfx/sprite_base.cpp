//
// Created by bobby on 22/03/2021.
//

#include "sprite_base.hpp"

#include <core/control_reciever.hpp>

#include <tonc.h>

void morpheus::core::gfx::SpriteBase::draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    if(obj_attr_num >= 128) {
        std::cout << "ERROR: obj_attr_num exceeded max of 127!";
    } else if(is_drawn_node()) {
        draw_node(obj_attr_buffer, obj_attr_num);
        if(std::is_base_of<core::gfx::AffineInterface, typeof(this)>::value) {
            nocash_puts("drawing affine");

            reinterpret_cast<core::gfx::AffineInterface*>(this)->draw_affine_node(obj_attr_buffer, obj_attr_num);
        }
    }
}
