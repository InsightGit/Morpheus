//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_CONTROLLERS_HPP
#define MORPHEUS_GBA_TEST_CONTROLLERS_HPP

#include <core/uncopyable.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class BlendingMode {
                OFF,
                USE_WEIGHTS,
                FADE_TO_WHITE,
                FADE_TO_BLACK
            };

            class BlendingController : Uncopyable {
            public:
                virtual void disable_backdrop_blending() = 0;
                virtual void enable_backdrop_blending(bool bottom) = 0;

                virtual void disable_background_blending(unsigned int background) = 0;
                virtual void enable_background_blending(bool bottom, unsigned int background) = 0;

                virtual void disable_object_blending() = 0;
                virtual void enable_object_blending(bool bottom) = 0;

                virtual BlendingMode get_blending_mode() = 0;
                virtual void set_blending_mode(BlendingMode blending_mode) = 0;

                virtual unsigned char get_blend_weight(bool bottom) const = 0;
                virtual void set_blend_weight(bool bottom, unsigned char weight) = 0;

                virtual unsigned char get_blend_fade() const = 0;
                virtual void set_blend_fade(unsigned char fade) = 0;
            protected:
                unsigned char get_blending_value(unsigned int background_num);

                const unsigned char TOP_BLEND_SHIFT = 0;
                const unsigned char BOTTOM_BLEND_SHIFT = 8;

                const unsigned short EVA_BLEND_MASK = 0x001F;
                const unsigned char EVA_BLEND_SHIFT = 0;

                const unsigned short EVB_BLEND_MASK = 0x1F00;
                const unsigned char EVB_BLEND_SHIFT = 8;

                const unsigned short Y_BLEND_MASK = 0x001F;
                const unsigned char Y_BLEND_SHIFT = 0;

                const unsigned char BG_0_BLENDING = 0x01;
                const unsigned char BG_1_BLENDING = 0x02;
                const unsigned char BG_2_BLENDING = 0x04;
                const unsigned char BG_3_BLENDING = 0x08;
                const unsigned char OBJ_BLENDING = 0x10;
                const unsigned char BD_BLENDING = 0x20;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_CONTROLLERS_HPP
