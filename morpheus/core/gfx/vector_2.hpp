//
// Created by bobby on 22/08/2020.
//

#ifndef MORPHEUS_VECTOR_2_HPP
#define MORPHEUS_VECTOR_2_HPP

#include <string>

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

                virtual ~Vector2() = default;

                int get_x() const {
                    return m_x;
                }

                int get_y() const {
                    return m_y;
                }

                std::string to_string() const {
                    return "(" + std::to_string(m_x) + "," + std::to_string(m_y) + ")";
                }

                // TODO(Bobby): Do all other operator overloads

                bool operator==(const Vector2 &other) const {
                    return (m_x == other.m_x) && (m_y == other.m_y);
                }

                bool operator!=(const Vector2 &other) const {
                    return (m_x != other.m_x) || (m_y != other.m_y);
                }
            private:
                int m_x;
                int m_y;
            };
        }
    }
}

#endif //MORPHEUS_VECTOR_2_HPP
