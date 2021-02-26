//
// Created by bobby on 21/12/2020.
//

#include "node.hpp"

void morpheus::core::Node::received_input(InputEvent input_event) {
    input(input_event);

    for(Node *child : m_children) {
        child->received_input(input_event);
    }
}

void morpheus::core::Node::received_update(unsigned char cycle_time) {
    update(cycle_time);

    for(Node *child : m_children) {
        child->received_update(cycle_time);
    }
}

int morpheus::core::Node::draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num,
                               unsigned int priority) {
    if(obj_attr_num >= 128) {
        std::cout << "ERROR: obj_attr_num exceeded max of 127!";

        return -1;
    }

    priority = std::min(priority, 3u);

    draw_node(obj_attr_buffer, static_cast<int>(obj_attr_num), static_cast<int>(priority));

    ++obj_attr_num;
    ++priority;

    unsigned int children_value = draw_children(obj_attr_buffer, obj_attr_num, priority);

    return static_cast<int>(children_value);
}

unsigned int morpheus::core::Node::draw_children(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num,
                                                 unsigned int priority) {
    for(core::Node *node : get_children()) {
        if(node != nullptr) {
            int new_obj_attr = node->draw(obj_attr_buffer, obj_attr_num, priority);

            if(new_obj_attr >= 0) {
                obj_attr_num = new_obj_attr;
            } else {
                break;
            }
        }
    }

    return obj_attr_num;
}
