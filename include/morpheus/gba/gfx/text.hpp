//
// Created by bobby on 09/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_TEXT_HPP
#define MORPHEUS_GBA_TEST_TEXT_HPP

#include <tonc.h>

#include <morpheus/core/gfx/text_base.hpp>
#include <morpheus/gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Text : public core::gfx::TextBase {
            public:
                /// Constructs a Text object and optionally allows the
                /// native tonc text engine API to be used instead of the
                /// cross-platform Morpheus expression text API. Note however
                /// that when the native tonc text engine API is being used,
                /// custom font support and other attributes will be
                /// unsupported. For more details, see the documentation of
                /// the parent morpheus::core::gfx::TextBase class.
                /// \param affine Whether this Text object is affine or not.
                /// (Only non-affine Text objects currently supported with
                /// Morpheus expression text API, which is enabled by default)
                /// \param background_num The background number of this Text
                /// object to display on. (Usually [0-3])
                /// \param cbb The [0-3] tile offset to load this Text object's
                /// tile graphics data onto.
                /// \param sbb The [0-31] tilemap offset to load this Text
                /// object's tilemap onto.
                /// \param gba_main_loop A pointer to the current GbaMainLoop.
                /// \param use_native_text_api Whether to use the "native" tonc
                /// text engine API (true) or the cross-platform Morpheus
                /// expression text API.
                Text(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                     GbaMainLoop *gba_main_loop, bool use_native_text_api = false) :
                    core::gfx::TextBase(affine, background_num, cbb, sbb, gba_main_loop, use_native_text_api) {}

                /// Destructs the Text object.
                virtual ~Text() = default;
            protected:
                void change_print_position(core::gfx::Vector2 print_pos) override {
                    tte_set_pos(print_pos.get_x(), print_pos.get_y());
                }

                void print_chars(std::string string, bool init)override;
            };

            /// \class morpheus::gba::gfx::Text
            /// The Game Boy Advance (GBA) implementation of
            /// morpheus::core::gfx::Text. Can use the
            /// cross-platform Morpheus expression text API or the "native"
            /// tonc text engine API. For more details about this class,
            /// consult the documentation of the aforementioned parent class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_TEXT_HPP
