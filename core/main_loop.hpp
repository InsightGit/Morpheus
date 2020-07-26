//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_MAIN_LOOP_HPP
#define MORPHEUS_MAIN_LOOP_HPP

#include "node.hpp"
#include "uncopyable.hpp"

namespace morpheus {
    namespace core {
        enum Error {
            OK
        };

        class MainLoop : Uncopyable {
        public:
            MainLoop() {
                if(platform_init() != OK) {
                    //
                }
            }

            virtual Error game_loop() = 0;
        protected:
            virtual Error platform_init() = 0;

            std::unique_ptr<Node> m_root;
        };
    }
}

#endif //MORPHEUS_MAIN_LOOP_HPP
