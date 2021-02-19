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

            // only called when Node is root of the tree.
            void received_input(InputEvent input_event);
            void received_update(unsigned char cycle_time);

            unsigned int draw(std::vector<void *>obj_attr_buffer, unsigned int obj_attr_num = 0,
                              unsigned int priority = 0);
        protected:
            std::list<Node*> get_children();

            virtual void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority) = 0;

            virtual void input(InputEvent input_event) = 0;
            virtual void update(unsigned char cycle_time) = 0;
        private:
            unsigned int draw_children(std::vector<void *>obj_attr_buffer, unsigned int obj_attr_num,
                                       unsigned int priority);

            std::list<std::unique_ptr<Node>> m_children;
        };
    }
}

#endif //MORPHEUS_NODE_HPP
