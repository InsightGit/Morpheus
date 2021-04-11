//
// Created by bobby on 05/02/2021.
//

#include <nds/nds.hpp>

int main() {
    std::shared_ptr<morpheus::nds::NdsMainLoop> main_loop(new morpheus::nds::NdsMainLoop());

    consoleInit(nullptr, 0, BgType_Text4bpp, BgSize_T_256x256,2, 2,
                true, true);

    if(isDSiMode()) {
        char string[8];

        unsigned int bytes_read = main_loop->get_save_manager()->load(&string[0], 8);
        std::string file_system_base_path = static_cast<morpheus::nds::DsiSdSaveManager*>(
                main_loop->get_save_manager())->get_file_system_base_path();

        if(bytes_read == 0) {
            unsigned int bytes_written;
            char save_string[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

            bytes_written = main_loop->get_save_manager()->save(&save_string[0], 8);

            std::cout << "saved " << bytes_written << " bytes to dsi\n on " << file_system_base_path;
        } else {
            std::cout << "read " << bytes_read << " bytes from dsi\n on " << file_system_base_path;
        }
    } else {
        std::cout << "This is not DSi mode!";
    }

    main_loop->game_loop();
}