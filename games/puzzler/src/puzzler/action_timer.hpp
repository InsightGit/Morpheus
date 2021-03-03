//
// Created by bobby on 26/02/2021.
//

#ifndef PUZZLER_ACTION_TIMER_HPP
#define PUZZLER_ACTION_TIMER_HPP

namespace puzzler {
    struct ActionTimer {
        unsigned char current_action_cycle = 0;
        bool current_action_cycle_waiting = false;
        int cycles_since = 0;
    };
}

#endif //PUZZLER_ACTION_TIMER_HPP
