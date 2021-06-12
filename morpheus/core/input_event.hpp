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


        /// \enum morpheus::core::InputButton
        /// An enum class that specifies all the possible input buttons
        /// that serve as user input in-game.

        enum class InputState {
            DOWN,
            UP,
            HELD
        };


        /// \enum morpheus::core::InputState
        /// An enum class that specifies all the possible input states
        /// an InputButton could be at. This includes the button being
        /// newly pressed (InputState::DOWN), the button being newly
        /// released (InputState::UP),
        /// or the button being held down (InputState::HELD).

        struct InputEvent {
            InputButton button;
            InputState state;
        };


        /// \struct morpheus::core::InputEvent
        /// A struct recieved by ControlRecievers and gfx::SpriteBases upon
        /// new or continued user input. The button being inputted is given
        /// under InputEvent::button, and the state of that button being
        /// inputted is given under InputEvent::state.
    }
}

#endif //MORPHEUS_INPUT_EVENT_HPP
