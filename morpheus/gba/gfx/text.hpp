//
// Created by bobby on 09/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_TEXT_HPP
#define MORPHEUS_GBA_TEST_TEXT_HPP

#include <tonc.h>

#include <core/gfx/text_base.hpp>
#include <gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Text : public core::gfx::TextBase {
            public:
                Text(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                     GbaMainLoop *gba_main_loop) :
                    core::gfx::TextBase(affine, background_num, cbb, sbb, gba_main_loop, false) {}

                virtual ~Text() = default;
            protected:
                void change_print_position(core::gfx::Vector2 print_pos) override {
                    tte_set_pos(print_pos.get_x(), print_pos.get_y());
                }

                void print_chars(std::string string, bool init)override;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_HPP
