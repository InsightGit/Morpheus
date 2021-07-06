//
// Created by bobby on 19/02/2021.
//

#ifndef MORPHEUS_NDS_TEST_TEXT_HPP
#define MORPHEUS_NDS_TEST_TEXT_HPP

#include <iostream>

#include <nds.h>

#include <morpheus/nds/nds_main_loop.hpp>
#include <morpheus/core/gfx/text_base.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            enum class TextBackgroundSize {
                BGSIZE_32_X_32,
                BGSIZE_64_X_32,
                BGSIZE_32_X_64,
                BGSIZE_64_X_64
            };

            class Text : public core::gfx::TextBase {
            public:
                Text(bool use_sub_display, bool affine, unsigned int background_num, unsigned int cbb,
                     unsigned int sbb, TextBackgroundSize background_size, NdsMainLoop *main_loop) :
                     core::gfx::TextBase(affine, background_num, cbb, sbb, main_loop, false) {
                    m_background_size = background_size;
                    m_use_sub_display = use_sub_display;

                    m_print_console = *consoleGetDefault();
                }

                virtual ~Text() = default;

                TextBackgroundSize get_background_size() const {
                    return m_background_size;
                }

                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }

                void set_background_size(const TextBackgroundSize background_size) {
                    m_background_size = background_size;
                }
            protected:
                void change_print_position(core::gfx::Vector2 print_pos) override {
                    std::cout << "\x1b[" << div32(print_pos.get_y(), 8) << ";" <<
                                 div32(print_pos.get_x(), 8) << "H a";
                }

                void print_chars(std::string string, bool init)override;
            private:
                TextBackgroundSize m_background_size;
                bool m_font_loaded = false;
                nds::NdsMainLoop *m_main_loop;
                PrintConsole m_print_console;
                bool m_use_sub_display;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_HPP
