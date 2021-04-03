//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_CONTROLLERS_HPP
#define MORPHEUS_GBA_TEST_CONTROLLERS_HPP

#include <array>

#include <core/uncopyable.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class BlendingMode {
                OFF,
                USE_WEIGHTS,
                FADE_TO_WHITE,
                FADE_TO_BLACK
            };

            enum class BlendingSetting {
                OFF,
                BOTTOM_ON,
                TOP_ON
            };

            class BlendingController : Uncopyable {
            public:
                virtual ~BlendingController() {}

                void disable_backdrop_blending() {
                    m_backdrop_blending_setting = BlendingSetting::OFF;

                    update_blending_registers();
                }

                void enable_backdrop_blending(bool bottom) {
                    if(bottom) {
                        m_backdrop_blending_setting = BlendingSetting::BOTTOM_ON;
                    } else {
                        m_backdrop_blending_setting = BlendingSetting::TOP_ON;
                    }

                    update_blending_registers();
                }

                void disable_background_blending(unsigned int background) {
                    if(background < 4) {
                        m_background_blending_settings[background] = BlendingSetting::OFF;
                    }

                    update_blending_registers();
                }

                void enable_background_blending(bool bottom, unsigned int background) {
                    if(background < 4) {
                        if(bottom) {
                            m_background_blending_settings[background] = BlendingSetting::BOTTOM_ON;
                        } else {
                            m_background_blending_settings[background] = BlendingSetting::TOP_ON;
                        }
                    }

                    update_blending_registers();
                }

                void disable_object_blending() {
                    m_object_blending_setting = BlendingSetting::OFF;

                    update_blending_registers();
                }

                void enable_object_blending(bool bottom) {
                    if(bottom) {
                        m_object_blending_setting = BlendingSetting::BOTTOM_ON;
                    } else {
                        m_object_blending_setting = BlendingSetting::TOP_ON;
                    }

                    update_blending_registers();
                }

                BlendingMode get_blending_mode() const {
                    return m_current_blending_mode;
                }

                void set_blending_mode(BlendingMode blending_mode) {
                    m_current_blending_mode = blending_mode;

                    update_blending_registers();
                }

                unsigned char get_blend_weight(bool bottom) const {
                    if(bottom) {
                        return m_bottom_blend_weight;
                    } else {
                        return m_top_blend_weight;
                    }
                }

                void set_blend_weight(bool bottom, unsigned char weight) {
                    if(bottom) {
                        m_bottom_blend_weight = std::min(weight, static_cast<unsigned char>(16));
                    } else {
                        m_top_blend_weight = std::min(weight, static_cast<unsigned char>(16));
                    }

                    update_blending_registers();
                }

                unsigned char get_blend_fade() const {
                    return m_blend_fade;
                }

                void set_blend_fade(unsigned char fade) {
                    m_blend_fade = std::max(fade, static_cast<unsigned char>(16));

                    update_blending_registers();
                }
            protected:
                BlendingSetting get_backdrop_blending_setting() const {
                    return m_backdrop_blending_setting;
                }

                std::array<BlendingSetting, 4> get_background_blending_settings() const {
                    return m_background_blending_settings;
                }

                BlendingSetting get_object_blending_setting() const {
                    return m_object_blending_setting;
                }

                unsigned char get_blending_value(unsigned int background_num);
                virtual void update_blending_registers() = 0;

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
            private:
                BlendingSetting m_backdrop_blending_setting;
                std::array<BlendingSetting, 4> m_background_blending_settings;
                unsigned char m_blend_fade;
                unsigned char m_bottom_blend_weight;
                BlendingMode m_current_blending_mode;
                BlendingSetting m_object_blending_setting;
                unsigned char m_top_blend_weight;
            };

            class MosaicController : Uncopyable {
            public:
                virtual ~MosaicController() {}

                Vector2 get_background_mosaic_levels() const {
                    return m_background_mosaic_levels;
                }

                Vector2 get_sprite_mosaic_levels() const {
                    return m_sprite_mosaic_levels;
                }

                void set_background_mosaic_levels(const Vector2 background_mosaic_levels) {
                    m_background_mosaic_levels = core::gfx::Vector2(
                                                        std::max(std::min(15, background_mosaic_levels.get_x()), 0),
                                                        std::max(std::min(15, background_mosaic_levels.get_y()), 0));

                    update_mosaic_register();
                }

                void set_sprite_mosaic_levels(const Vector2 sprite_mosaic_levels) {
                    m_sprite_mosaic_levels = core::gfx::Vector2(
                                                             std::max(std::min(15, sprite_mosaic_levels.get_x()), 0),
                                                             std::max(std::min(15, sprite_mosaic_levels.get_y()), 0));

                    update_mosaic_register();
                }
            protected:
                virtual void update_mosaic_register() = 0;
            private:
                Vector2 m_background_mosaic_levels;
                Vector2 m_sprite_mosaic_levels;
            };
        }

        class NoCashDebugController {
        public:
            virtual ~NoCashDebugController() = default;

            virtual void send_to_debug_window(std::string message) = 0;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_CONTROLLERS_HPP
