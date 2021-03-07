//
// Created by bobby on 07/02/2021.
//

#ifndef PUZZLER_SCENE_HPP
#define PUZZLER_SCENE_HPP

#include <vector>

namespace puzzler {
    class Scene : public morpheus::core::Node {
    public:
        virtual void setup() = 0;
    };
}

#endif //PUZZLER_SCENE_HPP
