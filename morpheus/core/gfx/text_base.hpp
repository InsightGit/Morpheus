//
// Created by bobby on 01/04/2021.
//

#ifndef MORPHEUS_TEXT_BASE_HPP
#define MORPHEUS_TEXT_BASE_HPP

#include <codecvt>
#include <locale>
#include <map>
#include <string>

#include <core/gfx/font.hpp>
#include <core/main_loop.hpp>
#include <core/gfx/tiled_background_base.hpp>
#include <core/gfx/vector_2.hpp>

//#include "sys8.h"

namespace morpheus {
    namespace core {
        namespace gfx {
            class TextBase {
            public:
                /// Constructs a TextBase object, which uses a background to
                /// display text. This can use Morpheus' cross-platform text
                /// API or the "native" text apis provided by libtonc or libnds
                /// on the GBA and the DS, respectively. Note that custom fonts
                /// and other advanced features through this class are only
                /// supported on the cross-platform Morpheus API.
                /// \param affine Whether this text background should be affine
                /// or not.
                /// \param background_num Which background this TextBase
                /// should use.
                /// \param cbb Which tile memory offset this TextBase should
                /// use.
                /// \param sbb Which tilemap memory offset this TextBase should
                /// use
                /// \param main_loop The pointer to the core::MainLoop currently
                /// being used
                /// \param use_native_text_api Whether to use the "native"
                /// text APIs (true) or the cross-platform Morpheus text API
                /// (false)
                TextBase(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                         morpheus::core::MainLoop *main_loop, bool use_native_text_api);

                /// Destructs this TextBase.
                virtual ~TextBase() = default;

                /// \return The background number this TextBase is currently using.
                unsigned int get_background_num() const {
                    return m_background_num;
                }

                /// \return The bounding box the text of this TextBase cannot
                /// extend beyond. If the x or y component is less than or
                /// equal to zero, that will mean the text of this TextBase
                /// will not be constrainted by that component.
                Vector2 get_bounding_box() const {
                    return m_bounding_box;
                }

                /// \return The tile memory offset this TextBase is currently
                /// using
                unsigned int get_cbb() const {
                    return m_cbb;
                }

                /// \return The current Font this Morpheus API-based
                /// TextBase is using. If this TextBase is based upon
                /// native text APIs, this returned value will be meaningless.
                Font get_current_font() const {
                    return m_font;
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

                void set_bounding_box(Vector2 bounding_box) {
                    m_bounding_box = bounding_box;
                }

                void set_current_font(const Font &font) {
                    m_font = font;
                }

                void set_print_position(Vector2 print_pos) {
                    m_print_position = print_pos;

                    m_cursor_position = m_print_position;

                    change_print_position(m_print_position);
                }

                void print_at_pos(std::string string, Vector2 print_pos) {
                    set_print_position(print_pos);

                    print(string);

                    set_print_position(m_print_position);
                }

                void print(std::string string, bool reinit = false) {
                    if(m_use_native_text_api) {
                        if(reinit || !m_inited) {
                            m_main_loop->enable_background(m_background_num);

                            print_chars(string, true);
                        } else {
                            print_chars(string, false);
                        }
                    } else {
                        if(reinit || !m_inited) {
                            init_expression_text_api();
                        }

                        expression_print_chars(string);
                    }

                    m_inited = true;
                }
            protected:
                virtual void change_print_position(Vector2 print_pos) = 0;
                virtual void print_chars(std::string string, bool init) = 0;

                void expression_print_chars(std::string string);

                bool init_expression_text_api();
            private:
                std::vector<int> get_tile_ids_from_ascii_string(const std::string &string);
                std::vector<int> get_tile_ids_from_utf_string(const std::string &string);

                bool m_affine;
                unsigned int m_background_num;
                Vector2 m_bounding_box = Vector2(0, 0);
                unsigned int m_cbb;
                Vector2 m_cursor_position;
                std::shared_ptr<core::gfx::TiledBackgroundBase> m_expression_background;
                Font m_font; //= get_default_font();
                bool m_inited;
                Vector2 m_print_position;
                core::MainLoop *m_main_loop;
                unsigned int m_sbb;
                bool m_use_native_text_api;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_BASE_HPP
