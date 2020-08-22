//
// Created by bobby on 22/08/2020.
//

#ifndef MORPHEUS_INPUT_EVENT_HPP
#define MORPHEUS_INPUT_EVENT_HPP

namespace morpheus {
    namespace core {
        enum class InputButton {
            NONE,
            DPADUP,
            DPADDOWN,
            DPADLEFT,
            DPADRIGHT,
            A,
            B,
            L,
            R,
            START,
            SELECT
        };

        enum class InputState {
            DOWN,
            UP,
            HELD
        };

        struct InputEvent {
            InputButton button;
            InputState state;
        };
    }
}

#endif //MORPHEUS_INPUT_EVENT_HPP
