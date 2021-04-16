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
            virtual bool is_successfully_mounted() const = 0;

            virtual unsigned int load(unsigned char *data, unsigned int len) = 0;
            virtual unsigned int save(const unsigned char *data, unsigned int len) = 0;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP
