//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_NODE_HPP
#define MORPHEUS_NODE_HPP

#include <iostream>
#include <list>
#include <memory>

#include <core/input_event.hpp>

namespace morpheus {
    namespace core {
        class ControlReciever {
        public:
            ControlReciever() = default;

            virtual ~ControlReciever() = default;

            virtual void input(const InputEvent input_event) = 0;
            virtual void update(const unsigned char cycle_time) = 0;
        };
    }
}

#endif //MORPHEUS_NODE_HPP
