//
// Created by bobby on 15/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_SRAM_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_GBA_SRAM_SAVE_MANAGER_HPP

#include <algorithm>
#include <cstring>

#include <tonc.h>

#include <core/save_manager.hpp>

#define SRAM (reinterpret_cast<unsigned char*>(MEM_SRAM))

namespace morpheus {
    namespace gba {
        class GbaSramSaveManager : public morpheus::core::SaveManager {
        public:
            GbaSramSaveManager() = default;

            virtual ~GbaSramSaveManager() = default;

            bool is_successfully_mounted() const override {
                return true;
            }

            unsigned int load(unsigned char *data, unsigned int len)override;
            unsigned int save(const unsigned char *data, unsigned int len)override;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_SRAM_SAVE_MANAGER_HPP
