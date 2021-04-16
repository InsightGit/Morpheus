//
// Created by bobby on 16/04/2021.
//

#include "gba_eeprom_save_manager.hpp"

morpheus::gba::GbaEepromSaveManager::GbaEepromSaveManager(EepromSize eeprom_size) {
    // sets wait state 2 first and second access to 8 cycles as required by EEPROM
    // and as detailed in GBATEK
    // 0000 0011 0000 0000
    REG_WAITCNT |= 0x0300;

    m_eeprom_size = eeprom_size;
}

unsigned int morpheus::gba::GbaEepromSaveManager::load(unsigned char *data, unsigned int len) {
    unsigned int seek_0_bit_stream;
    
    return 0;
}

unsigned int morpheus::gba::GbaEepromSaveManager::save(const unsigned char *data, unsigned int len) {
    return 0;
}

bool morpheus::gba::GbaEepromSaveManager::seek_to_eeprom_address(const unsigned int block_number) {
    // As detailed in GBATEK here(https://problemkaputt.de/gbatek.htm#gbacartbackupeeprom) set the address for reading
    unsigned int seek_bit_stream = (EEPROM_READ_REQUEST << 14) | block_number;

    // bounds checking
    switch (m_eeprom_size) {
        case EepromSize::EEPROM_512_BYTES:
            if(block_number > 63){
                return false;
            }
            break;
        case EepromSize::EEPROM_8_KILOBYTES:
            if(block_number > 1023) {
                return false;
            }
            break;
    }

    memcpy16(reinterpret_cast<void*>(EEPROM_START), &seek_bit_stream, 1);

    return true;
}
