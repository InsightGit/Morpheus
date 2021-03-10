//
// Created by bobby on 07/02/2021.
//

#ifndef PUZZLER_SCENE_HPP
#define PUZZLER_SCENE_HPP

#include <vector>

namespace puzzler {
    class Scene : public morpheus::core::Node {
    public:
        bool is_marked_for_deletion() const {
            return m_deletion;
        }

        virtual void setup() = 0;
    protected:
        void mark_for_deletion() {
            m_deletion = true;
        }
    private:
        bool m_deletion = false;
    };
}

#endif //PUZZLER_SCENE_HPP
