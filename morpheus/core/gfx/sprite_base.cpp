//
// Created by bobby on 22/03/2021.
//

#include "sprite_base.hpp"

#include <core/control_reciever.hpp>

void morpheus::core::gfx::SpriteBase::draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    if(obj_attr_num >= 128) {
        std::cout << "ERROR: obj_attr_num exceeded max of 127!";
    } else if(is_drawn_node()) {
        draw_node(obj_attr_buffer, static_cast<int>(obj_attr_num));

        ++obj_attr_num;
    }
}
