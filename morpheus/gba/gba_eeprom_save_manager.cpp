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
    unsigned int actual_len = 0;
    unsigned int current_block_num = 0;
    unsigned int current_byte = 0;
    unsigned int tmp_eeprom_read_buffer[3];

    switch(m_eeprom_size){
        case EepromSize::EEPROM_512_BYTES:
            actual_len = std::min(512u, len);
            break;
        case EepromSize::EEPROM_8_KILOBYTES:
            actual_len = std::min(8192u, len);
            break;
    }

    while(current_byte < actual_len) {
        if(!read_seek_to_eeprom_address(current_block_num)) {
            return current_byte;
        }

        // As detailed in GBATEK here(https://problemkaputt.de/gbatek.htm#gbacartbackupeeprom) read the
        // 68 bit data appropriately and discard the first four bits to get the 64 bit (or 8 byte) data stored
        // on EEPROM
        // TODO(Bobby): Verify bit-twiddling on these two DMA transfers
        dma_cpy(tmp_eeprom_read_buffer, EEPROM_START, 2, 3, DMA_CPY32);
        dma_cpy(&tmp_eeprom_read_buffer[2], EEPROM_START, 1, 3, DMA_CPY16);

        tmp_eeprom_read_buffer[0] = (tmp_eeprom_read_buffer[0] << 4) | ((tmp_eeprom_read_buffer[1] & 0xF000) >> 12);
        tmp_eeprom_read_buffer[1] = (tmp_eeprom_read_buffer[1] & 0x0FFF) | ((tmp_eeprom_read_buffer[2] & 0xF000) >> 12);

        for (int i = 0; 8 > i; ++i) {
            unsigned int eeprom_value;

            if (current_byte >= actual_len) {
                break;
            }

            if (i > 4) {
                eeprom_value = tmp_eeprom_read_buffer[1];
            } else {
                eeprom_value = tmp_eeprom_read_buffer[0];
            }

            data[i] = (eeprom_value >> (i * 8)) & 0x000000FF;

            ++current_byte;
        }

        ++current_block_num;
    }
    
    return len;
}

unsigned int morpheus::gba::GbaEepromSaveManager::save(const unsigned char *data, unsigned int len) {
    // len NEEDS to be a multiple of eight bytes
    unsigned int actual_len = 0;
    unsigned int current_block_num = 0;
    unsigned int current_byte = 0;
    unsigned int write_request_bit_length;

    switch(m_eeprom_size) {
        case EepromSize::EEPROM_512_BYTES:
            actual_len = std::min(512u, len);
            write_request_bit_length = 8;

            break;
        case EepromSize::EEPROM_8_KILOBYTES:
            actual_len = std::min(8192u, len);
            write_request_bit_length = 16;

            break;
    }

    // uses the EEPROM write process as detailed in GBATEK here(https://problemkaputt.de/gbatek.htm#gbacartbackupeeprom)
    while(current_byte < actual_len) {
        // preparing the write request bit stream
        unsigned int write_request_header[3];

        write_request_header[0] = EEPROM_WRITE_REQUEST | (current_block_num << 2);
        write_request_header[1] = 0x00000000;
        write_request_header[2] = 0x00000000;

        if(current_byte + 8 >= actual_len) {
            return current_byte;
        }

        for(unsigned int i = 0; 8 > i; ++i) {
            unsigned int data_shift_base = (i * 8) + write_request_bit_length;

            if(data_shift_base < 32) {
                write_request_header[0] |= data[i] << data_shift_base;
            } else if(data_shift_base < 64){
                write_request_header[1] |= data[i] << (data_shift_base - 32);
            } else {
                write_request_header[2] |= data[i] << (data_shift_base - 64);
            }
        }

        // copying over the write request bit stream
        dma_cpy(EEPROM_START, write_request_header, 3, 3, DMA_CPY32);

        while() {
            //
        }

        current_byte += 8;
    }

    return 0;
}

bool morpheus::gba::GbaEepromSaveManager::read_seek_to_eeprom_address(const unsigned int block_number) {
    // As detailed in GBATEK here(https://problemkaputt.de/gbatek.htm#gbacartbackupeeprom) set the address for reading
    unsigned int seek_bit_stream;

    switch (m_eeprom_size) {
        case EepromSize::EEPROM_512_BYTES:
            if(block_number > 63){
                return false;
            }

            seek_bit_stream = EEPROM_READ_REQUEST | (block_number << 2) | (0 << 7);
            break;
        case EepromSize::EEPROM_8_KILOBYTES:
            if(block_number > 1023) {
                return false;
            }

            seek_bit_stream = EEPROM_READ_REQUEST | (block_number << 2) | (0 << 15);
            break;
    }

    dma_cpy(EEPROM_START, &seek_bit_stream, 1, 3, DMA_CPY16);

    return true;
}
