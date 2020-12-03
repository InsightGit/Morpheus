//
// Created by bobby on 22/08/2020.
//

#ifndef MORPHEUS_INPUT_EVENT_HPP
#define MORPHEUS_INPUT_EVENT_HPP

namespace morpheus {
    namespace core {
        enum class InputButton {
            /* GBA/DS keys */
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
            SELECT,
            /* DS exclusive keys */
            X,
            Y,
            TOUCH,
            LID
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
