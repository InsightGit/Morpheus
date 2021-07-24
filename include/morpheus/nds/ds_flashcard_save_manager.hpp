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
            /// Constructs a DsFlashcardSaveManager object with a given base
            /// path for the save file, and whether the libfat filesystem has
            /// been successfully mounted by the NdsMainLoop.
            /// \param file_system_base_path The already existing base path
            /// within the libfat filesystem to store the save file in
            /// \param successfully_mounted Whether the NdsMainLoop was
            /// successful in mounting the libfat filesystem
            explicit DsFlashcardSaveManager(std::string file_system_base_path, bool successfully_mounted) {
                m_file_system_base_path = file_system_base_path;
                m_successfully_mounted = successfully_mounted;
            }

            /// Destructs the DsFlashcardSaveManager object.
            virtual ~DsFlashcardSaveManager() = default;

            bool is_successfully_mounted() const override {
                return m_successfully_mounted;
            }

            /// \return The base path within the libfat filesystem to store
            /// the save file in.
            std::string get_file_system_base_path() const {
                return m_file_system_base_path;
            }

            unsigned int load(unsigned char *data, unsigned int len) override {
                return load("morpheus.sav", data, len);
            }

            unsigned int save(const unsigned char *data, unsigned int len) override {
                return save("morpheus.sav", data, len);
            }

            /// Loads some data from the non-volatile libfat filesystem that this
            /// save manager is using under a certain file name.
            /// \param file_name The file name of the save file
            /// \param data The data buffer to load the save data into
            /// \param len The length of the save data buffer
            /// \return The amount of bytes loaded into the data buffer
            unsigned int load(std::string file_name, unsigned char *data, unsigned int len);

            /// Saves some data to the non-volatile libfat filesystem that this
            /// save manager is using under a certain file name.
            /// \param file_name The file name of the save file
            /// \param data The save data buffer
            /// \param len The length of the save data buffer
            /// \return The amount of bytes saved into non-volatile memory
            unsigned int save(std::string file_name, const unsigned char *data, unsigned int len);
        private:
            std::string m_file_system_base_path;
            FILE *m_save_file_pointer;
            bool m_successfully_mounted = false;
        };

        /// \class morpheus::nds::DsFlashcardSaveManager
        /// DLDI libfat implementation of morpheus::core::SaveManager.
        /// Saves save files (by default called morpheus.sav) to the SD card of
        /// DLDI-supported flashcards.
    }
}

#endif //MORPHEUS_GBA_TEST_D_SI_SD_SAVE_MANAGER_HPP
