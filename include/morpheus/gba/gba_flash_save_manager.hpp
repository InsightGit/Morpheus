//
// Created by bobby on 18/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_FLASH_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_GBA_FLASH_SAVE_MANAGER_HPP

#pragma GCC diagnostic warning "-Wstringop-overflow"

#include <ctime>

#include <tonc.h>

#include <morpheus/core/save_manager.hpp>

namespace morpheus {
    namespace gba {
        enum class FlashSaveSize {
            SIZE_AUTO_DETECT, ///< Auto detection of the size of the current
                              ///< flash chip
            SIZE_64_KB, ///< 64 kilobyte flash chips
            SIZE_128_KB ///< 128 kilobyte flash chips
        };

        /// \enum morpheus::gba::FlashSaveSize
        /// An enum class representing the possible sizes of non-volatile flash
        /// memory on the GBA.

        class GbaFlashSaveManager : public core::SaveManager {
        public:
            /// Constructs a GbaFlashSaveManager object with a given
            /// FlashSaveSize.
            /// \param flash_save_size The size of the non-volatile flash chip
            /// that this game will use.
            GbaFlashSaveManager(FlashSaveSize flash_save_size);

            /// Destructs the GbaFlashSaveManager object.
            virtual ~GbaFlashSaveManager() = default;

            bool is_successfully_mounted() const override {
                return true;
            }

            unsigned int load(unsigned char *data, unsigned int len)override;
            unsigned int save(const unsigned char *data, unsigned int len)override;
        private:
            unsigned char *FLASH_DEVICE_REGISTER_1 = reinterpret_cast<unsigned char *>(0x0E005555);
            unsigned char *FLASH_DEVICE_REGISTER_2 = reinterpret_cast<unsigned char *>(0x0E002AAA);

            unsigned char *FLASH_START = reinterpret_cast<unsigned char *>(0x0E000000);

            bool is_ateml() const {
                return m_manufacturer_id == 0x1F;
            }

            void bank_switch(bool bank_zero);

            unsigned char m_device_id;
            FlashSaveSize m_flash_save_size;
            unsigned char m_manufacturer_id;
        };

        /// \class morpheus::gba::GbaFlashSaveManager
        /// Non-volatile GBA flash chip implementation of
        /// morpheus::core::SaveManager. Saves non-volatile save data to a
        /// GBA-specific 64 or 128 kilobyte non-volatile flash chip.
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_FLASH_SAVE_MANAGER_HPP
