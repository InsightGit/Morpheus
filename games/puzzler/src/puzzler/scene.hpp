//
// Created by bobby on 07/02/2021.
//

#ifndef PUZZLER_SCENE_HPP
#define PUZZLER_SCENE_HPP

#include <core/node.hpp>

namespace puzzler {
    class Scene : public morpheus::core::Node {
    public:
        Scene(morpheus::core::MainLoop *main_loop) {
            set_drawn_node(false);

            m_main_loop = main_loop;
        }

        bool is_marked_for_deletion() const {
            return m_deletion;
        }

        virtual ~Scene() = default;

        virtual void setup() = 0;
    protected:
        morpheus::core::MainLoop *get_main_loop() const {
            return m_main_loop;
        }

        void mark_for_deletion() {
            m_deletion = true;
        }
    private:
        bool m_deletion = false;
        morpheus::core::MainLoop *m_main_loop;
    };
}

#endif //PUZZLER_SCENE_HPP
