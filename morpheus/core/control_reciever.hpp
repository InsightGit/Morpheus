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
            /// Constructs a ControlReciever.
            ControlReciever() = default;

            /// Destructs a ControlReciever
            virtual ~ControlReciever() = default;

            /// Recieve a InputEvent for processing within this ControlReciever.
            /// \param input_event The InputEvent to be recieved by this ControlReciever
            virtual void input(const InputEvent input_event) = 0;

            /// Updates this ControlReciever's internal state.
            /// This should be called no more than once per VBlank.
            /// \param cycle_time The current frame number (0-59) after a VBlank
            virtual void update(const unsigned char cycle_time) = 0;
        };


        /// \class morpheus::core::ControlReciever
        /// A abstract class on which Sprites and other objects requiring
        /// user input or VBlank updating should use. Needs to be added to the
        /// MainLoop using MainLoop::add_control_reciever before input() and
        /// update() are called on a regular basis.
    }
}

#endif //MORPHEUS_NODE_HPP
