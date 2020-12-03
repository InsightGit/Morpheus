//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <list>
#include <memory>

#include <tonc.h>

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

            std::list<Node*> get_children() {
                std::list<Node*> return_value;

                for(std::unique_ptr<Node> &child : m_children) {
                    return_value.push_back(child.get());
                }

                return return_value;
            }

            void received_input(InputEvent input_event) {
                input(input_event);

                for(std::unique_ptr<Node> &child : m_children) {
                    child->input(input_event);
                }
            }

            virtual void draw(OBJ_ATTR (*obj_attr_buffer)[], int obj_attr_num = 0) = 0;
        protected:
            virtual void draw_children(OBJ_ATTR (*obj_attr_buffer)[], int obj_attr_num) = 0;

            virtual void input(InputEvent input_event) = 0;
        private:
            std::list<std::unique_ptr<Node>> m_children;
        };
    }
}

#endif //MORPHEUS_NODE_HPP
