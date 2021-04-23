//
// Created by bobby on 05/04/2021.
//

#include "ds_flashcard_save_manager.hpp"

unsigned int morpheus::nds::DsFlashcardSaveManager::load(std::string file_name, unsigned char *data, unsigned int len) {
    unsigned int return_value;

    m_save_file_pointer = fopen(std::string(m_file_system_base_path + file_name).c_str(), "rb");

    if(m_save_file_pointer == nullptr) {
        return_value = 0;
    } else {
        return_value = fread(data, len, 1, m_save_file_pointer);

        if(return_value == 1) {
            return_value = len;
        }

        fclose(m_save_file_pointer);
    }

    return return_value;
}


unsigned int morpheus::nds::DsFlashcardSaveManager::save(std::string file_name, const unsigned char *data,
                                                         unsigned int len) {
    unsigned int return_value;

    m_save_file_pointer = fopen(std::string(m_file_system_base_path + file_name).c_str(), "wb");

    if(m_save_file_pointer == nullptr) {
        std::cout << "Couldn't save to save file\n";
        return_value = 0;
    } else {
        return_value = fwrite(data, len, 1, m_save_file_pointer);

        if(return_value == 1) {
            return_value = len;
        }

        fclose(m_save_file_pointer);
    }

    return return_value;
}
