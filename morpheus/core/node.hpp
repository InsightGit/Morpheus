//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <iostream>
#include <list>
#include <memory>

#include <core/main_loop.hpp>
#include <core/input_event.hpp>

namespace morpheus {
    namespace core {
        class Node {
        public:
            Node() = default;

            void add_child(Node *child) {
                m_children.push_back(std::unique_ptr<Node>(child));
            }

            void remove_child(Node *child){
                // TODO(Bobby): Implement this method
            }

            void received_input(InputEvent input_event) {
                input(input_event);

                for(std::unique_ptr<Node> &child : m_children) {
                    child->input(input_event);
                }
            }

            unsigned int draw(std::vector<void *>obj_attr_buffer, unsigned int obj_attr_num = 0,
                              unsigned int priority = 0) {
                if(obj_attr_num >= 128) {
                    std::cout << "ERROR obj_attr_num exceeded max of 127!";

                    return -1;
                }

                priority = std::min(priority, 3u);

                draw_node(obj_attr_buffer, static_cast<int>(obj_attr_num), static_cast<int>(priority));

                ++obj_attr_num;

                return obj_attr_num + draw_children(obj_attr_buffer, obj_attr_num, priority + 1);
            }
        protected:
            std::list<Node*> get_children() {
                std::list<Node*> return_value;

                for(std::unique_ptr<Node> &child : m_children) {
                    return_value.push_back(child.get());
                }

                return return_value;
            }

            virtual void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority) = 0;

            virtual void input(InputEvent input_event) = 0;
        private:
            unsigned int draw_children(std::vector<void *>obj_attr_buffer, unsigned int obj_attr_num,
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

            std::list<std::unique_ptr<Node>> m_children;
        };
    }
}

#endif //MORPHEUS_NODE_HPP
