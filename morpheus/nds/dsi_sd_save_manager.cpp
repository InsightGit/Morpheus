//
// Created by bobby on 05/04/2021.
//

#include "dsi_sd_save_manager.hpp"

unsigned int morpheus::nds::DsiSdSaveManager::load(std::string file_name, void *data, unsigned int len) {
    unsigned int return_value;

    m_save_file_pointer = fopen(std::string(m_file_system_base_path + file_name).c_str(), "r");

    if(m_save_file_pointer == nullptr) {
        std::cout << "Couldn't load save file\n";

        return_value = 0;
    } else {
        return_value = fread(data, len, 1, m_save_file_pointer);

        fclose(m_save_file_pointer);
    }

    return return_value;
}


unsigned int morpheus::nds::DsiSdSaveManager::save(std::string file_name, void *data, unsigned int len) {
    unsigned int return_value;

    m_save_file_pointer = fopen(std::string(m_file_system_base_path + file_name).c_str(), "w");

    if(m_save_file_pointer == nullptr) {
        std::cout << "Couldn't save to save file\n";
        return_value = 0;
    } else {
        return_value = fwrite(data, len, 1, m_save_file_pointer);

        fclose(m_save_file_pointer);
    }

    return return_value;
}