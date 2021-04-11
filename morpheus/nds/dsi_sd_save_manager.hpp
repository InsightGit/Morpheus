//
// Created by bobby on 05/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_DSI_SD_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_DSI_SD_SAVE_MANAGER_HPP

#include <iostream>

#include <fat.h>
#include <filesystem.h>
#include <nds.h>

#include <core/save_manager.hpp>

namespace morpheus {
    namespace nds {
        class DsiSdSaveManager : public core::SaveManager {
        public:
            explicit DsiSdSaveManager(std::string file_system_base_path) {
                m_file_system_base_path = file_system_base_path;
            }

            virtual ~DsiSdSaveManager() = default;

            std::string get_file_system_base_path() const {
                return m_file_system_base_path;
            }

            int load(void *data, unsigned int len) override {
                return load("morpheus.sav", data, len);
            }

            int save(void *data, unsigned int len) override {
                return save("morpheus.sav", data, len);
            }

            int load(std::string file_name, void *data, unsigned int len);
            int save(std::string file_name, void *data, unsigned int len);
        private:
            std::string m_file_system_base_path;
            FILE *m_save_file_pointer;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_D_SI_SD_SAVE_MANAGER_HPP
