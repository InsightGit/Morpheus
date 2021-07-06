//
// Created by bobby on 15/04/2021.
//

#include "morpheus/gba/gba_sram_save_manager.hpp"

unsigned int morpheus::gba::GbaSramSaveManager::load(unsigned char *data, unsigned int len) {
    unsigned int bytes_to_load = std::min(len, static_cast<unsigned int>(SRAM_SIZE));

    std::memcpy(data, SRAM, bytes_to_load);

    return bytes_to_load;
}

unsigned int morpheus::gba::GbaSramSaveManager::save(const unsigned char *data, unsigned int len) {
    unsigned int bytes_to_save = std::min(len, static_cast<unsigned int>(SRAM_SIZE));

    std::memcpy(SRAM, data, bytes_to_save);

    return bytes_to_save;
}
