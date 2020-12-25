//
// Created by bobby on 24/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_BACKGROUND_HPP
#define MORPHEUS_GBA_TEST_BACKGROUND_HPP

#include <algorithm>

#include <tonc.h>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Background {
            public:
                explicit Background(unsigned int background_num);

                void load_from_array(unsigned short *tiles, unsigned short *palette, unsigned short );

                void draw();
            private:
                int m_background_num;
                unsigned short m_bg_control_register;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_BACKGROUND_HPP
