//
// Created by bobby on 18/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_FLASH_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_GBA_FLASH_SAVE_MANAGER_HPP

#include <ctime>

#include <tonc.h>

#include <core/save_manager.hpp>

namespace morpheus {
    namespace gba {
        enum class FlashSaveSize {
            SIZE_AUTO_DETECT,
            SIZE_64_KB,
            SIZE_128_KB
        };

        class GbaFlashSaveManager : public core::SaveManager {
        public:
            GbaFlashSaveManager(FlashSaveSize flash_save_size);

            virtual ~GbaFlashSaveManager() = default;

            bool is_successfully_mounted() const {
                return true;
            }

            unsigned int load(unsigned char *data, unsigned int len);
            unsigned int save(const unsigned char *data, unsigned int len);
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
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_FLASH_SAVE_MANAGER_HPP
