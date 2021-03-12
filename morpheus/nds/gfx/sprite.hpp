//
// Created by bobby on 16/12/2020.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_HPP
#define MORPHEUS_GBA_TEST_SPRITE_HPP

#include <nds.h>

#include <core/core.hpp>

#pragma GCC diagnostic ignored "-Wunused-variable"

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

            enum class ExtendedPaletteStatus {
                NEEDED,
                NOTNEEDED,
                NEEDOFF
            };

            static OamStatus OAM_STATUS = OamStatus::DISABLED;

            class Sprite : public core::Node {
            public:
                explicit Sprite(bool use_sub_display, SpriteMapping sprite_mapping,
                                ExtendedPaletteStatus external_palette);
                explicit Sprite(bool use_sub_display, SpriteMapping sprite_mapping,
                                ExtendedPaletteStatus external_palette, unsigned short *nds_oam_address,
                                const unsigned char width, const unsigned char height);

                virtual ~Sprite();

                uint16_t *get_gfx_pointer() const {
                    return m_gfx_pointer;
                }

                core::gfx::Vector2 get_position() const {
                    return m_position;
                }

                void set_position(const core::gfx::Vector2 position) {
                    m_position = position;
                }

                void set_position(const int x, const int y) {
                    set_position(core::gfx::Vector2(x, y));
                }

                unsigned int get_palette_id() const {
                    return m_palette_id;
                }

                void set_palette_id(const unsigned int palette_id) {
                    m_palette_id = palette_id;
                }

                // Extended palette load functions
                virtual bool load_from_array(const unsigned short *tile_array, const unsigned int palette_id,
                                             const unsigned int width, const unsigned int height) = 0;
                virtual bool load_from_array(const unsigned short *tile_array, const unsigned short *palette,
                                             const unsigned int palette_len, const unsigned int palette_id,
                                             const unsigned int width, const unsigned int height) = 0;
                virtual bool load_into_palette(const unsigned short *palette, const unsigned int pal_len) = 0;
            protected:
                void allocate_gfx_pointer(SpriteColorFormat color_format, uint8_t width = 0, uint8_t height = 0);

                OamState *get_current_oam() const {
                    return m_current_oam;
                }

                SpriteSize get_sprite_size() const {
                    return m_sprite_size;
                }

                bool is_in_extended_palette_mode() const {
                    return m_extended_palette;
                }

                void set_sprite_size(const unsigned char width, const unsigned char height);

                void on_visible_state_changed(bool hidden) override {
                    if(m_last_used_obj_attr_num != -1) {
                        oamSetHidden(m_current_oam, m_last_used_obj_attr_num, hidden);
                    }
                }

                virtual void update(unsigned char cycle_time) override {}
                virtual void input(core::InputEvent input_event) override {}
            protected:
                void set_last_used_obj_attr_num(const int last_used_obj_attr_num) {
                    m_last_used_obj_attr_num = last_used_obj_attr_num;
                }
            private:
                bool m_do_not_free_gfx_pointer;
                bool m_extended_palette;
                bool m_enabled = false;
                int m_last_used_obj_attr_num = -1;
                unsigned int m_palette_id;

                core::gfx::Vector2 m_position;

                OamState *m_current_oam;
                uint16_t *m_gfx_pointer = nullptr;
                SpriteSize m_sprite_size;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_HPP
