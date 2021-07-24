//
// Created by bobby on 15/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_SRAM_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_GBA_SRAM_SAVE_MANAGER_HPP

#include <algorithm>
#include <cstring>

#include <tonc.h>

#include <morpheus/core/save_manager.hpp>

#define SRAM (reinterpret_cast<unsigned char*>(MEM_SRAM))

namespace morpheus {
    namespace gba {
        class GbaSramSaveManager : public morpheus::core::SaveManager {
        public:
            /// Constructs a GbaSramSaveManager object.
            GbaSramSaveManager() = default;

            /// Destructs the GbaSramSaveManager object.
            virtual ~GbaSramSaveManager() = default;

            bool is_successfully_mounted() const override {
                return true;
            }

            unsigned int load(unsigned char *data, unsigned int len)override;
            unsigned int save(const unsigned char *data, unsigned int len)override;
        };

        /// \class morpheus::gba::GbaSramSaveManager
        /// GBA FRAM/SRAM implementation of morpheus::core::SaveManager.
        /// Saves non-volatile save data to non-volatile 32 kilobyte
        /// SRAM or FRAM.
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_SRAM_SAVE_MANAGER_HPP
