//
// Created by bobby on 01/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_TEXT_BASE_HPP
#define MORPHEUS_GBA_TEST_TEXT_BASE_HPP

#include <core/main_loop.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            class TextBase {
            public:
                // TODO(Bobby): Add custom font support maybe?
                TextBase(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                         morpheus::core::MainLoop *main_loop);

                virtual ~TextBase() = default;

                unsigned int get_background_num() const {
                    return m_background_num;
                }

                unsigned int get_cbb() const {
                    return m_cbb;
                }

                Vector2 get_print_position() const {
                    return m_print_position;
                }

                unsigned int get_sbb() const {
                    return m_sbb;
                }

                bool is_affine() const {
                    return m_affine;
                }

                void set_print_position(Vector2 print_pos) {
                    m_print_position = print_pos;

                    change_print_position(m_print_position);
                }

                void print_at_pos(std::string string, Vector2 print_pos) {
                    change_print_position(print_pos);

                    print(string);

                    change_print_position(m_print_position);
                }

                void print(std::string string, bool reinit = false) {
                    if(reinit || !m_inited) {
                        m_main_loop->enable_background(m_background_num);

                        print_chars(string, true);
                    } else {
                        print_chars(string, false);
                    }

                    m_inited = true;
                }
            protected:
                virtual void change_print_position(Vector2 print_pos) = 0;
                virtual void print_chars(std::string string, bool init) = 0;
            private:
                bool m_affine;
                unsigned int m_background_num;
                unsigned int m_cbb;
                bool m_inited;
                Vector2 m_print_position;
                core::MainLoop *m_main_loop;
                unsigned int m_sbb;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_BASE_HPP
