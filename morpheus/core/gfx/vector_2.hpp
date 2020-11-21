//
// Created by bobby on 22/08/2020.
//

#ifndef MORPHEUS_VECTOR_2_HPP
#define MORPHEUS_VECTOR_2_HPP

namespace morpheus {
    namespace core {
        namespace gfx {
            class Vector2 {
            public:
                Vector2(int x, int y) {
                    m_x = x;
                    m_y = y;
                }

                Vector2() : Vector2(0, 0) {}

                int get_x() const {
                    return m_x;
                }

                int get_y() const {
                    return m_y;
                }
            private:
                int m_x;
                int m_y;
            };
        }
    }
}

#endif //MORPHEUS_VECTOR_2_HPP
