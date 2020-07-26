//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_UNCOPYABLE_HPP
#define MORPHEUS_UNCOPYABLE_HPP

namespace morpheus {
    namespace core {
        class Uncopyable {
        protected:
            Uncopyable() {}
            ~Uncopyable() {}
        private:
            Uncopyable(const Uncopyable&) = delete;
            void operator=(const Uncopyable&) = delete;
        };
    }
}


#endif //MORPHEUS_UNCOPYABLE_HPP
