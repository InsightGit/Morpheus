//
// Created by bobby on 05/02/2021.
//

#ifdef _GBA
#include <morpheus/gba/gba.hpp>
#elif _NDS
#include <morpheus/nds/nds.hpp>
#endif

int main() {
    #ifdef GBA_EEPROM_SAVE
        std::shared_ptr<morpheus::core::MainLoop> main_loop(
                morpheus::utils::construct_appropriate_main_loop(morpheus::core::GbaSaveType::EEPROM_8KB));
    #elif GBA_FLASH_SAVE
        std::shared_ptr<morpheus::core::MainLoop> main_loop(
                morpheus::utils::construct_appropriate_main_loop(morpheus::core::GbaSaveType::FLASH_128KB));
    #else
        std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    #endif
    std::shared_ptr<morpheus::core::gfx::TextBase> text_base(
            morpheus::utils::construct_appropriate_text(false, 0, 3, 3, main_loop.get(), false));

    if(main_loop->get_save_manager()->is_successfully_mounted()) {
        unsigned char save_string[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', '\0'};
        unsigned char string[8];

        unsigned int bytes_read = main_loop->get_save_manager()->load(&string[0], 8);
        unsigned int bytes_written;

        std::string platform_detail_string;

        #ifdef _NDS
            platform_detail_string = "on " + platform_detail_string + " path";
        #elif GBA_EEPROM_SAVE
            platform_detail_string = "on GBA EEPROM";
        #elif GBA_FLASH_SAVE
            platform_detail_string = "on GBA FLASH";
        #else
            platform_detail_string = "on GBA SRAM";
        #endif

        if(bytes_read > 0) {
            text_base->print("read " + std::to_string(bytes_read) + " bytes from non volatile\n" +
                             "mem " + platform_detail_string + "\ninit byte is " +
                             std::to_string(static_cast<uint8_t>(string[0])) +
                             std::string(" bytes are\n") + reinterpret_cast<char*>(string) +
                             " strlen is " + std::to_string(strlen(reinterpret_cast<char*>(string))) + "\n");
        }

        bytes_written = main_loop->get_save_manager()->save(&save_string[0], 8);

        text_base->print("saved " + std::to_string(bytes_written) + " bytes to non volatile mem\n" +
                                platform_detail_string + "\n");
    } else {
        text_base->print("The save manager wasn't successfully mounted!\n");
    }

    main_loop->enable_background(1);

    main_loop->game_loop();
}