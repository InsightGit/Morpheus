//
// Created by bobby on 16/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_EEPROM_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_GBA_EEPROM_SAVE_MANAGER_HPP

#include <tonc.h>

#include <core/save_manager.hpp>


namespace morpheus {
    namespace gba {
        enum class EepromSize {
            EEPROM_512_BYTES,
            EEPROM_8_KILOBYTES
        };

        class GbaEepromSaveManager : public morpheus::core::SaveManager {
        public:
            GbaEepromSaveManager(EepromSize eeprom_size);

            bool is_successfully_mounted() const override {
                return true;
            }

            unsigned int load(unsigned char *data, unsigned int len)override;
            unsigned int save(const unsigned char *data, unsigned int len)override;
        private:
            unsigned char *EEPROM_START = reinterpret_cast<unsigned char*>(0xDFFFF00);

            const unsigned char EEPROM_READ_REQUEST = 0x0003;
            const unsigned char EEPROM_WRITE_REQUEST = 0x0002;

            bool read_seek_to_eeprom_address(const unsigned int block_number);

            EepromSize m_eeprom_size;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_EEPROM_SAVE_MANAGER_HPP
