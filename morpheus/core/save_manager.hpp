//
// Created by bobby on 05/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP

#include <string>
#include <vector>

namespace morpheus {
    namespace core {
        class SaveManager {
        public:
            virtual unsigned int load(void *data, unsigned int len) = 0;
            virtual unsigned int save(void *data, unsigned int len) = 0;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP
