//
// Created by bobby on 16/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_HPP
#define MORPHEUS_GBA_TEST_SPRITE_HPP

#include <nds.h>

#include <core/core.hpp>

namespace morpheus {
    namespace nds {
        namespace gfx {
            enum class OamStatus {
                DISABLED,
                ENABLED,
                ENABLED_EXTENDED,
                ENABLED_BITMAP,
                ENABLED_EXTENDED_BITMAP
            };

            static OamStatus OAM_STATUS = OamStatus::DISABLED;

            class Sprite : public core::Node {
            public:
                explicit Sprite(bool use_sub_display, SpriteMapping sprite_mapping, bool extended_palette);

                virtual ~Sprite();

                core::gfx::Vector2 get_position() const {
                    return m_position;
                }

                void set_position(const core::gfx::Vector2 position) {
                    m_position = position;
                }

                void set_position(const int x, const int y) {
                    set_position(core::gfx::Vector2(x, y));
                }

                /*// Single palette load functions
                virtual bool load_from_array(uint8_t **tile_array, uint8_t width, uint8_t height) = 0;
                virtual bool load_from_array(uint8_t **tile_array, uint16_t **palette,
                                             uint8_t width, uint8_t height) = 0;

                // Extended palette load functions
                virtual bool load_from_array(uint8_t **tile_array, uint8_t palette_id, uint8_t width,
                                             uint8_t height) = 0;
                virtual bool load_from_array(uint8_t **tile_array, uint8_t palette_id,
                                             uint16_t **palette, uint8_t width, uint8_t height) = 0;*/
            protected:
                void allocate_gfx_pointer(SpriteColorFormat color_format, uint8_t width = 0, uint8_t height = 0) {
                    if(width > 0 && height > 0) {
                        set_sprite_size(width, height);
                    }

                    if(m_gfx_pointer != nullptr) {
                        std::cout << "freeing gfx\n";

                        oamFreeGfx(m_current_oam, m_gfx_pointer);
                    }

                    m_gfx_pointer = oamAllocateGfx(m_current_oam, m_sprite_size, color_format);
                }

                OamState *get_current_oam() const {
                    return m_current_oam;
                }

                uint16_t *get_gfx_pointer() const {
                    return m_gfx_pointer;
                }

                SpriteSize get_sprite_size() const {
                    return m_sprite_size;
                }

                bool is_in_extended_palette_mode() const {
                    return m_use_extended_palette;
                }

                void set_sprite_size(uint8_t width, uint8_t height);

                virtual void input(core::InputEvent input_event)override {}
            private:
                bool m_use_extended_palette;

                core::gfx::Vector2 m_position;

                OamState *m_current_oam;
                uint16_t *m_gfx_pointer = nullptr;
                SpriteSize m_sprite_size;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_HPP
