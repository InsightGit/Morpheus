//
// Created by bobby on 16/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_GBA_EEPROM_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_GBA_EEPROM_SAVE_MANAGER_HPP

#include <bitset>

#include <tonc.h>

#include <morpheus/core/save_manager.hpp>

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
            unsigned short *EEPROM_START = reinterpret_cast<unsigned short*>(0x0D000000);

            const unsigned char EEPROM_READ_REQUEST = 0x03;
            const unsigned char EEPROM_WRITE_REQUEST = 0x02;

            static unsigned int lsb_short_to_msb_short(unsigned int lsb_short) {
                return ((lsb_short & 0xFF00) >> 8) | ((lsb_short & 0x00FF) << 8);
            }

            bool read_seek_to_eeprom_address(const unsigned int block_number);

            EepromSize m_eeprom_size;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_GBA_EEPROM_SAVE_MANAGER_HPP
