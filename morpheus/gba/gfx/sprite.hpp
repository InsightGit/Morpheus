//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_SPRITE_HPP
#define MORPHEUS_SPRITE_HPP

#include <cassert>
#include <cstring>

#include <tonc.h>

#include <core/core.hpp>

#include <gba/gba_controllers.hpp>
#include <gba/gba_main_loop.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite : public core::gfx::SpriteBase {
                public:
                    explicit Sprite(const bool is_4bpp, GbaBlendingController *blending_controller) :
                                    core::gfx::SpriteBase(blending_controller) {
                        m_is_4bpp = is_4bpp;
                    }

                    explicit Sprite(const bool is_4bpp, const unsigned short palette_id, const unsigned short tile_id,
                                    core::gfx::SpriteSize size, GbaBlendingController *blending_controller) :
                                    Sprite(is_4bpp, blending_controller) {
                        build_attr2(palette_id, tile_id);
                        set_sprite_size(size);
                    }

                    bool is_blended() const override {
                        return m_blended;
                    }

                    virtual ~Sprite();

                    void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette, const unsigned int palette_len,
                                         const core::gfx::SpriteSize size, const unsigned int tile_id);

                    void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const core::gfx::SpriteSize size, const unsigned int tile_id);
                protected:
                    core::gfx::Vector2 get_mosaic_levels() const override {
                        return m_mosaic_levels;
                    }

                    void set_mosaic_levels(const core::gfx::Vector2 mosaic_levels) override {
                        m_mosaic_levels = core::gfx::Vector2(std::max(std::min(15, mosaic_levels.get_x()), 0),
                                                             std::max(std::min(15, mosaic_levels.get_y()), 0));

                        REG_MOSAIC &= ~(MOS_OH_MASK | MOS_OV_MASK);

                        nocash_puts("setting mosaic to");
                        nocash_puts(m_mosaic_levels.to_string().c_str());

                        if(m_mosaic_levels.get_x() > 0 && m_mosaic_levels.get_y() > 0) {
                            REG_MOSAIC |= MOS_OH(m_mosaic_levels.get_x()) | MOS_OV(m_mosaic_levels.get_y());
                        } else if(m_mosaic_levels.get_x() > 0) {
                            REG_MOSAIC |= MOS_OH(m_mosaic_levels.get_x());
                        } else if(m_mosaic_levels.get_y() > 0) {
                            REG_MOSAIC |= MOS_OV(m_mosaic_levels.get_y());
                        }
                    }

                    void build_attr2(const unsigned short palette_id, const unsigned short tile_id) {
                        m_attr2 = ATTR2_BUILD(tile_id, palette_id, 0);
                    }

                    void toggle_blending(bool enable_blending, bool bottom_layer) override {
                        get_blending_controller()->disable_object_blending();

                        if(enable_blending) {
                            m_attr0 |= ATTR0_BLEND;
                            m_blended = true;

                            get_blending_controller()->enable_object_blending(bottom_layer);
                        } else {
                            m_attr0 &= ~ATTR0_BLEND;
                            m_blended = false;
                        }
                    }

                    void set_sprite_size(core::gfx::SpriteSize size)override;

                    void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority)override;
                    void mosaic_state_updated()override;
                    void on_visible_state_changed(bool hidden)override;

                    virtual void input(core::InputEvent input_event)override {}
                    virtual void update(unsigned char cycle_time)override {}

                    virtual void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                            const core::gfx::SpriteSize size, const unsigned int tile_id) = 0;
                    virtual void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                            const unsigned short *palette, const unsigned int palette_len,
                                            const core::gfx::SpriteSize size, const unsigned int tile_id) = 0;
                private:
                    bool m_blended = false;
                    bool m_is_4bpp;
                    int m_last_obj_attr_num;
                    core::gfx::Vector2 m_mosaic_levels = core::gfx::Vector2(1, 1);
                    unsigned int m_priority = 0;

                    unsigned short m_attr0;
                    unsigned short m_attr1;
                    unsigned short m_attr2;
            };
        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
