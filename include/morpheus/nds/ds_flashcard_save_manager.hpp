//
// Created by bobby on 05/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_DS_FLASHCARD_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_DS_FLASHCARD_SAVE_MANAGER_HPP

#include <iostream>

#include <fat.h>
#include <filesystem.h>
#include <nds.h>

#include <morpheus/core/save_manager.hpp>

namespace morpheus {
    namespace nds {
        class DsFlashcardSaveManager : public core::SaveManager {
        public:
            explicit DsFlashcardSaveManager(std::string file_system_base_path, bool successfully_mounted) {
                m_file_system_base_path = file_system_base_path;
                m_successfully_mounted = successfully_mounted;
            }

            virtual ~DsFlashcardSaveManager() = default;

            bool is_successfully_mounted() const override {
                return m_successfully_mounted;
            }

            std::string get_file_system_base_path() const {
                return m_file_system_base_path;
            }

            unsigned int load(unsigned char *data, unsigned int len) override {
                return load("morpheus.sav", data, len);
            }

            unsigned int save(const unsigned char *data, unsigned int len) override {
                return save("morpheus.sav", data, len);
            }

            unsigned int load(std::string file_name, unsigned char *data, unsigned int len);
            unsigned int save(std::string file_name, const unsigned char *data, unsigned int len);
        private:
            std::string m_file_system_base_path;
            FILE *m_save_file_pointer;
            bool m_successfully_mounted = false;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_D_SI_SD_SAVE_MANAGER_HPP
