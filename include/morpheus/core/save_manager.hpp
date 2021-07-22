//
// Created by bobby on 05/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP
#define MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP

#include <string>
#include <vector>

namespace morpheus {
    namespace core {
        class SaveManager {
        public:
            /// \return Whether the non-volatile memory this SaveManager
            /// uses is successfully mounted.
            virtual bool is_successfully_mounted() const = 0;

            /// Loads the save data from the non-volatile memory this
            /// SaveManager uses.
            /// \param data The buffer to load the save data into
            /// \param len The length (in bytes) of the data buffer
            /// \return The number of bytes read (or loaded)
            virtual unsigned int load(unsigned char *data, unsigned int len) = 0;

            /// Saves the save data to the non-volatile memory this SaveManager uses.
            /// \param data The buffer of the save data to be saved in
            /// non-volatile memory
            /// \param len The length (in bytes) of the data buffer
            /// \return The number of bytes written (or saved)
            virtual unsigned int save(const unsigned char *data, unsigned int len) = 0;
        };


        /// \class morpheus::core::SaveManager
        /// An abstract interface for writing game saves to non-volatile
        /// memory.
    }
}

#endif //MORPHEUS_GBA_TEST_SAVE_MANAGER_HPP
