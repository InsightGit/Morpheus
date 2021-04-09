//
// Created by bobby on 19/02/2021.
//

#ifndef MORPHEUS_GBA_TEST_TEXT_HPP
#define MORPHEUS_GBA_TEST_TEXT_HPP

#include <nds.h>

#include <core/gfx/text_base.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            enum class BackgroundSize {
                BGSIZE_32_X_32,
                BGSIZE_64_X_32,
                BGSIZE_32_X_64,
                BGSIZE_64_X_64
            };

            class Text : public core::gfx::TextBase {
            public:
                Text(bool use_sub_display, bool affine, unsigned int background_num, unsigned int cbb,
                     unsigned int sbb) : core::gfx::TextBase(affine, background_num, cbb, sbb) {
                    m_use_sub_display = use_sub_display;
                }

                virtual ~Text() = default;

                BackgroundSize get_background_size() const {
                    return m_background_size;
                }

                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }

                void set_background_size(const BackgroundSize background_size) {
                    m_background_size = background_size;
                }

                void print(std::string string)override;
            protected:
                void change_print_position(core::gfx::Vector2 print_pos) override {
                    // TODO
                    //tte_set_pos(print_pos.get_x(), print_pos.get_y());
                }
            private:
                BackgroundSize m_background_size;
                bool m_font_loaded = false;
                PrintConsole m_print_console;
                bool m_use_sub_display;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_HPP
