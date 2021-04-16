//
// Created by bobby on 05/02/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    #ifdef _GBA
        tte_init_se(1, BG_CBB(5) | BG_SBB(5) | BG_PRIO(0), 0, CLR_WHITE, 14, nullptr, nullptr);
    #elif _NDS
        consoleInit(nullptr, 0, BgType_Text4bpp, BgSize_T_256x256,2, 2,
                true, true);
    #endif

    if(main_loop->get_save_manager()->is_successfully_mounted()) {
        unsigned char string[8];
        unsigned char save_string[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', '\0'};

        unsigned int bytes_read = main_loop->get_save_manager()->load(&string[0], 8);

        std::string platform_detail_string;

        #ifdef _NDS
            platform_detail_string = "on " + platform_detail_string + " path";
        #elif GBA_EEPROM_SAVE
            platform_detail_string = "on GBA EEPROM";
        #else
            platform_detail_string = "on GBA SRAM";
        #endif

        if (bytes_read == 0 || string[0] != save_string[0]) {
            unsigned int bytes_written;

            bytes_written = main_loop->get_save_manager()->save(&save_string[0], 8);

            #ifdef _GBA
                tte_write(std::string("saved " + std::to_string(bytes_written) + " bytes to non volatile mem\n" +
                                      platform_detail_string + "\n").c_str());
            #else
                std::cout << "saved " << bytes_written << " bytes to non volatile mem\n" << platform_detail_string <<
                             "\n";
            #endif
        } else {
            #ifdef _GBA
                tte_write(std::string("read " + std::to_string(bytes_read) + " bytes from non volatile mem\n" +
                                           platform_detail_string + "\ninit byte is " +
                                           std::to_string(static_cast<uint8_t>(string[0])) +
                                           std::string("\nbytes are ") + reinterpret_cast<char*>(string)).c_str());
            #else
                std::cout << "read " << bytes_read << " bytes from non volatile mem\n" << platform_detail_string <<
                             "\n" << "bytes are " << string;
            #endif
        }
    } else {
        #ifdef _GBA
            tte_write("The save manager wasn't successfully mounted!\n");
        #else
            std::cout << "The save manager wasn't successfully mounted!\n";
        #endif
    }

    main_loop->enable_background(1);

    main_loop->game_loop();
}