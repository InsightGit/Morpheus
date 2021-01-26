//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_UTILS_HPP
#define MORPHEUS_GBA_TEST_UTILS_HPP

#ifdef _GBA
    #include <gba/gba.hpp>
#elif _NDS
    #include <nds/nds.hpp>
#else
    #error Unsupported Platform!
#endif

namespace morpheus {
    class Utils {
    public:
        static morpheus::core::MainLoop* construct_appropriate_main_loop() {
            #ifdef _GBA
                return new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::OFF);
            #elif _NDS
                return new morpheus::nds::NdsMainLoop(morpheus::nds::DebugConsoleMode::ON_SUB);
            #endif

            return nullptr;
        }
    };
}


#endif //MORPHEUS_GBA_TEST_UTILS_HPP
