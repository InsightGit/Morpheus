//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <list>
#include <memory>

namespace morpheus {
    namespace core {
        class Node {
        public:
            Node() {}

            void add_child(Node *child) {
                m_children.push_back(std::unique_ptr<Node>(child));
            }

            void draw_children() {
                for(std::unique_ptr<Node> &child : m_children) {
                    child.get()->draw();
                }
            }

            std::list<Node*> get_children() {
                std::list<Node*> return_value;

                for(std::unique_ptr<Node> &child : m_children) {
                    return_value.push_back(child.get());
                }

                return return_value;
            }

            virtual void draw() = 0;
        private:
            std::list<std::unique_ptr<Node>> m_children;
        };
    }
}

#endif //MORPHEUS_NODE_HPP
