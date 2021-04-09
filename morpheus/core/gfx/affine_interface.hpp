//
// Created by bobby on 07/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_AFFINE_INTERFACE_HPP
#define MORPHEUS_GBA_TEST_AFFINE_INTERFACE_HPP

#include <type_traits>
#include <typeinfo>
#include <vector>

#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            class AffineInterface {
            public:
                virtual int get_rotation() const = 0;
                virtual Vector2 get_scale() const = 0;

                virtual void set_rotation(const int rotation) = 0;
                virtual void set_scale(const core::gfx::Vector2 scale) = 0;

                virtual void draw_affine_node(std::vector<void *> &obj_attr_buffer,
                                              unsigned int obj_attr_num) = 0;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_AFFINE_INTERFACE_HPP
