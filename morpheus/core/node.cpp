//
// Created by bobby on 21/12/2020.
//


#include "node.hpp"

void morpheus::core::Node::received_input(InputEvent input_event) {
    input(input_event);

    for(std::unique_ptr<Node> &child : m_children) {
        child->received_input(input_event);
    }
}

void morpheus::core::Node::received_update(unsigned char cycle_time) {
    update(cycle_time);

    for(std::unique_ptr<Node> &child : m_children) {
        child->received_update(cycle_time);
    }
}

unsigned int morpheus::core::Node::draw(std::vector<void *> obj_attr_buffer, unsigned int obj_attr_num,
                                        unsigned int priority) {
    if(obj_attr_num >= 128) {
        std::cout << "ERROR: obj_attr_num exceeded max of 127!";

        return -1;
    }

    priority = std::min(priority, 3u);

    draw_node(obj_attr_buffer, static_cast<int>(obj_attr_num), static_cast<int>(priority));

    ++obj_attr_num;

    return obj_attr_num + draw_children(obj_attr_buffer, obj_attr_num, priority + 1);
}

std::list<morpheus::core::Node *> morpheus::core::Node::get_children() {
    std::list<Node*> return_value;

    for(std::unique_ptr<Node> &child : m_children) {
        return_value.push_back(child.get());
    }

    return return_value;
}

unsigned int morpheus::core::Node::draw_children(std::vector<void *> obj_attr_buffer, unsigned int obj_attr_num,
                                                 unsigned int priority) {
    for(core::Node *node : get_children()) {
        if(node->draw(obj_attr_buffer, obj_attr_num, priority) >= 0) {
            ++obj_attr_num;
        } else {
            break;
        }
    }

    return obj_attr_num;
}
