//
// Created by bobby on 16/04/2021.
//

#include "morpheus/gba/gba_eeprom_save_manager.hpp"

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
    unsigned short tmp_eeprom_read_buffer[5];

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
        dma_cpy(tmp_eeprom_read_buffer, EEPROM_START, 5, 3, DMA_CPY16 | DMA_NOW | DMA_ENABLE );
        //dma_cpy(&tmp_eeprom_read_buffer[2], EEPROM_START, 1, 3, DMA_CPY16);

        tte_write("read buffer bitstreams:\n");

        for(int i = 0; 3 > i; ++i) {
            tte_write(std::bitset<32>(tmp_eeprom_read_buffer[0]).to_string().c_str());
            tte_write("\n");
        }

        /*tmp_eeprom_read_buffer[0] = (tmp_eeprom_read_buffer[0] >> 4) | ((tmp_eeprom_read_buffer[1] & 0x000F) << 12);
        tmp_eeprom_read_buffer[1] = (tmp_eeprom_read_buffer[1] & 0xFFF0 >> 4) |
                                    ((tmp_eeprom_read_buffer[2] & 0x000F) << 12);*/

        for(int i = 0; 4 > i; ++i) {
            tmp_eeprom_read_buffer[i] = (tmp_eeprom_read_buffer[i] >> 4) |
                                        ((tmp_eeprom_read_buffer[i + 1] & 0x000F) << 12);
        }

        for (int i = 0; 8 > i; ++i) {
            unsigned int eeprom_value;

            if (current_byte >= actual_len) {
                break;
            }

            if (i > 5) {
                eeprom_value = tmp_eeprom_read_buffer[3];
            } else if (i > 3) {
                eeprom_value = tmp_eeprom_read_buffer[2];
            } else if (i > 1) {
                eeprom_value = tmp_eeprom_read_buffer[1];
            } else {
                eeprom_value = tmp_eeprom_read_buffer[0];
            }

            if((i & 0x0001) == 1) {
                // odd index

                data[i] = (eeprom_value >> 8) & 0x000000FF;
            } else {
                // even index

                data[i] = (eeprom_value >> 0) & 0x000000FF;
            }

            ++current_byte;
        }

        ++current_block_num;
    }
    
    return current_byte;
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

    nocash_puts(std::string("actual len to be read from EEPROM: " + std::to_string(actual_len)).c_str());

    // uses the EEPROM write process as detailed in GBATEK here(https://problemkaputt.de/gbatek.htm#gbacartbackupeeprom)
    while(current_byte < actual_len) {
        // preparing the write request bit stream
        unsigned int write_request_header[3];

        write_request_header[0] = (EEPROM_WRITE_REQUEST << 0) | (lsb_short_to_msb_short(current_block_num) << 2);

        if(current_byte + 8 > actual_len) {
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

        nocash_puts("write request bitsets:");

        for(int i = 0; 3 > i; ++i) {
            nocash_puts(std::bitset<32>(write_request_header[i]).to_string().c_str());
        }

        // copying over the write request bit stream
        dma_cpy(EEPROM_START, write_request_header, 6, 3, DMA_CPY16 | DMA_NOW | DMA_ENABLE);

        unsigned int cycle_counter = 0;
        unsigned int ready_cache = 0;

        // wait until ready bit is set or return if cycle_counter has exceeded 10 ms (based off of the GBA's 16.78Mhz
        // CPU instruction speed)
        while((ready_cache & 0x0001) == 0) {
            ++cycle_counter;

            dma_cpy(&ready_cache, EEPROM_START, 1, 3, DMA_CPY16 | DMA_NOW | DMA_ENABLE);

            if(cycle_counter >= 167800) {
                tte_write(std::string("EEPROM timeout on byte " + std::to_string(current_byte) + "\n").c_str());

                return current_byte;
            }
        }

        current_byte += 8;
    }

    return current_byte;
}

bool morpheus::gba::GbaEepromSaveManager::read_seek_to_eeprom_address(const unsigned int block_number) {
    // As detailed in GBATEK here(https://problemkaputt.de/gbatek.htm#gbacartbackupeeprom) set the address for reading
    unsigned int seek_bit_stream;

    switch (m_eeprom_size) {
        case EepromSize::EEPROM_512_BYTES:
            if(block_number > 63){
                return false;
            }

            seek_bit_stream = (EEPROM_READ_REQUEST << 0) | (lsb_short_to_msb_short(block_number) << 2) | (0 << 7);
            break;
        case EepromSize::EEPROM_8_KILOBYTES:
            if(block_number > 1023) {
                return false;
            }

            seek_bit_stream = (EEPROM_READ_REQUEST << 0) | (lsb_short_to_msb_short(block_number) << 2) | (0 << 15);
            break;
    }

    nocash_puts("read seek bitstream: ");
    nocash_puts(std::bitset<16>(seek_bit_stream).to_string().c_str());

    dma_cpy(EEPROM_START, &seek_bit_stream, 1, 3, DMA_CPY16 | DMA_NOW | DMA_ENABLE);

    return true;
}
