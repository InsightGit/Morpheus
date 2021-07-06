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
                /// Constructs a Vector2 with a specified x and y component in a
                /// Cartesian coordinate space.
                /// \param x The x (horizontal) component to set
                /// \param y The y (vertical) component to set
                Vector2(int x, int y) {
                    m_x = x;
                    m_y = y;
                }

                /// Constructs a default Vector2 with a zero x and y component.
                Vector2() : Vector2(0, 0) {}

                /// Destructs a Vector2
                virtual ~Vector2() = default;

                /// \return The x component of this Vector2.
                int get_x() const {
                    return m_x;
                }

                /// \return The y component of this Vector2.
                int get_y() const {
                    return m_y;
                }

                /// \return The string representation of this Vector2 in the
                /// form (x,y)
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

                Vector2 operator+(const Vector2 &other) const {
                    return Vector2(m_x + other.m_x, m_y + other.m_y);
                }

                Vector2 operator-(const Vector2 &other) const {
                    return Vector2(m_x - other.m_x, m_y - other.m_y);
                }

                Vector2 operator*(const Vector2 &other) const {
                    return Vector2(m_x * other.m_x, m_y * other.m_y);
                }

                Vector2 operator/(const Vector2 &other) const {
                    return Vector2(m_x / other.m_x, m_y / other.m_y);
                }

                Vector2 operator%(const Vector2 &other) const {
                    return Vector2(m_x % other.m_x, m_y % other.m_y);
                }
            private:
                int m_x;
                int m_y;
            };


            /// \class morpheus::core::gfx::Vector2
            /// An utility class representing a plain 2D Vector in a
            /// Cartesian coordinate system.
        }
    }
}

#endif //MORPHEUS_VECTOR_2_HPP
