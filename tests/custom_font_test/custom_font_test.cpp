//
// Created by bobby on 22/04/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "japanese_font_test.h"

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    #ifdef _GBA
        TFont japanese_font;

        japanese_font.bpp = 1;
        japanese_font.charCount = 36;
        japanese_font.charOffset = 32;
        japanese_font.cellH = 16;
        japanese_font.cellW = 16;
        japanese_font.cellSize = 32;
        japanese_font.charH = 16;
        japanese_font.charW = 16;
        japanese_font.data = japanese_font_testTiles;
        japanese_font.extra = 0;
        japanese_font.heights = nullptr;
        japanese_font.widths = nullptr;

        tte_init_se(0, BG_CBB(5) | BG_SBB(5), 0, CLR_WHITE,14, &japanese_font,
                    nullptr);

        std::string tte_string;

        for(char i = 32; 68 > i; ++i) {
            if((i - 32) > 0 && (i - 32) % 8 == 0) {
                tte_string += "\n";
            }

            tte_string += i;
        }

        tte_write(tte_string.c_str());
    #elif _NDS
        // intentional copy
        PrintConsole japanese_print_console = *consoleGetDefault();
        ConsoleFont japanese_font;

        japanese_font.gfx = reinterpret_cast<unsigned short *>(japanese_font_testTiles);
        japanese_font.bpp = 4;
        japanese_font.asciiOffset = 32;
        japanese_font.numChars = 36;
        japanese_font.convertSingleColor = true;

        japanese_print_console.font = japanese_font;
        japanese_print_console.bgId = 0;
        japanese_print_console.gfxBase = 5;
        japanese_print_console.mapBase = 5;

        std::string tte_string;

        for(char i = 32; 68 > i; ++i) {
            tte_string += i;
        }

        std::cout << tte_string << "\n";
    #endif

    main_loop->enable_background(0);

    main_loop->game_loop();
}
