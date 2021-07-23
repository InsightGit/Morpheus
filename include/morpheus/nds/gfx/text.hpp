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
                BGSIZE_32_X_32, ///< 32 tiles x 32 tiles (or 256 px x 256 px)
                                ///< text background
                BGSIZE_64_X_32, ///< 64 tiles x 32 tiles (or 512 px x 256 px)
                                ///< text background
                BGSIZE_32_X_64, ///< 32 tiles x 64 tiles (or 256 px x 512 px)
                                ///< text background
                BGSIZE_64_X_64 ///< 64 tiles x 32 tiles (or 512 px x 512 px)
                               ///< text background
            };

            /// \enum morpheus::nds::gfx::TextBackgroundSize
            /// An enum class that specifies the size of the Text object's
            /// background.

            class Text : public core::gfx::TextBase {
            public:
                /// Constructs a Text object and optionally allows the
                /// native libnds PrintConsole API to be used instead of the
                /// cross-platform Morpheus expression text API. Note however
                /// that when the native PrintConsole API is being used,
                /// custom font support and other attributes will be
                /// unsupported. For more details, see the documentation of
                /// the parent morpheus::core::gfx::TextBase class.
                /// \param use_sub_display Whether to display this Text object
                /// on the bottom sub display (true) or the top main display
                /// (false).
                /// \param affine Whether this Text object is affine or not.
                /// (Only non-affine Text objects currently supported with
                /// Morpheus expression text API, which is enabled by default)
                /// \param background_num The background number of this Text
                /// object to display on. (Usually [0-3])
                /// \param cbb The [0-3] tile offset to load this Text object's
                /// tile graphics data onto.
                /// \param sbb The [0-31] tilemap offset to load this Text
                /// object's tilemap onto.
                /// \param background_size The background size of this Text
                /// object
                /// \param main_loop A pointer to the current NdsMainLoop.
                /// \param use_native_text_api Whether to use the "native" tonc
                /// text engine API (true) or the cross-platform Morpheus
                /// expression text API.
                Text(bool use_sub_display, bool affine, unsigned int background_num, unsigned int cbb,
                     unsigned int sbb, TextBackgroundSize background_size, NdsMainLoop *main_loop,
                     bool use_native_text_api = false) :
                     core::gfx::TextBase(affine, background_num, cbb, sbb, main_loop, false) {
                    m_background_size = background_size;
                    m_use_sub_display = use_sub_display;

                    m_print_console = *consoleGetDefault();
                }

                /// Destructs the Text object.
                virtual ~Text() = default;

                /// \return The TextBackgroundSize of this Text object.
                TextBackgroundSize get_background_size() const {
                    return m_background_size;
                }

                /// \return Whether this Text object is displaying on the
                /// bottom sub display (true) or the top main display (false).
                bool is_using_sub_display() const {
                    return m_use_sub_display;
                }

                /// Sets the background size of this Text object.
                /// \param background_size
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

            /// \class morpheus::nds::gfx::Text
            /// The Nintendo DS (NDS) implementation of
            /// morpheus::core::gfx::Text. Can use the
            /// cross-platform Morpheus expression text API or the "native"
            /// libnds PrintConsole API. For more details about this class,
            /// consult the documentation of the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_HPP
