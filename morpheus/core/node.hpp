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

            virtual ~Node() = default;

            void add_child(Node *child) {
                m_children.push_back(child);
            }

            void remove_child(Node *child){
                // TODO(Bobby): Implement this method
                for(unsigned int i = 0; m_children.size() > i; ++i) {
                    if(m_children[i] == child) {
                        m_children.erase(m_children.begin() + static_cast<int>(i));
                    }
                }
            }

            // only called when Node is root of the tree.
            void received_input(InputEvent input_event);
            void received_update(unsigned char cycle_time);

            bool is_hidden() {
                return m_hidden;
            }

            void hide() {
                m_hidden = true;

                on_visible_state_changed(m_hidden);
            }

            void show() {
                m_hidden = false;

                on_visible_state_changed(m_hidden);
            }

            int draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num = 0,
                     unsigned int priority = 0);
        protected:
            std::vector<Node*> get_children() const {
                return m_children;
            }

            bool is_drawn_node() const {
                return m_drawn_node;
            }

            void set_drawn_node(const bool drawn_node) {
                m_drawn_node = drawn_node;
            }

            virtual void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) = 0;
            virtual void input(InputEvent input_event) = 0;
            virtual void on_visible_state_changed(bool new_visible_state) = 0;
            virtual void update(unsigned char cycle_time) = 0;
        private:
            unsigned int draw_children(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num,
                                       unsigned int priority);

            std::vector<Node*> m_children;
            bool m_drawn_node = true;
            bool m_hidden = false;
        };
    }
}

#endif //MORPHEUS_NODE_HPP
