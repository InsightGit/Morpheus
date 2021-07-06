//
// Created by bobby on 25/07/2020.
//

#ifndef MORPHEUS_SPRITE_HPP
#define MORPHEUS_SPRITE_HPP

#include <cassert>
#include <cstring>
#include <iomanip>

#include <tonc.h>

#include <morpheus/core/core.hpp>

#include <morpheus/gba/gba_controllers.hpp>
#include <morpheus/gba/gba_main_loop.hpp>
#include <morpheus/gba/gfx/gba_animation_frame.hpp>

namespace morpheus {
    namespace gba {
        namespace gfx {
            class Sprite : public core::gfx::SpriteBase {
                public:
                    explicit Sprite(const bool affine, const bool is_4bpp, GbaBlendingController *blending_controller,
                                    GbaMosaicController *mosaic_controller) :
                                    core::gfx::SpriteBase(affine, blending_controller, mosaic_controller) {
                        m_is_4bpp = is_4bpp;

                        if(is_affine()) {
                            obj_aff_identity(&m_affine_current);

                            update_affine_state(core::gfx::AffineTransformation::Identity, true);
                        }
                    }

                    explicit Sprite(const bool affine, const bool is_4bpp, const unsigned short palette_id,
                                    const unsigned short tile_id, core::gfx::SpriteSize size,
                                    GbaBlendingController *blending_controller,
                                    GbaMosaicController *mosaic_controller) : Sprite(affine, is_4bpp,
                                                                                     blending_controller,
                                                                                     mosaic_controller) {
                        build_attr2(palette_id, tile_id);
                        set_sprite_size(size);
                    }

                    ~Sprite() override;

                    void build_attr2(const unsigned short palette_id, const unsigned short tile_id) {
                        m_attr2 = ATTR2_BUILD(tile_id, palette_id, 0);
                    }

                    unsigned int get_palette_id() const {
                        return (m_attr2 & ATTR2_PALBANK_MASK) >> ATTR2_PALBANK_SHIFT;
                    }

                    core::gfx::SpriteSize get_sprite_size() const override {
                        return m_sprite_size;
                    }

                    unsigned int get_tile_id() const {
                        return (m_attr2 & ATTR2_ID_MASK) >> ATTR2_ID_SHIFT;
                    }

                    bool is_blended() const override {
                        return m_blended;
                    }

                    void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette, const unsigned int palette_len,
                                         const core::gfx::SpriteSize size, const unsigned int tile_id);

                    void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const core::gfx::SpriteSize size, const unsigned int tile_id);

                    void set_sprite_size(core::gfx::SpriteSize size)override;
                protected:
                    unsigned int get_attr0() const {
                        return m_attr0;
                    }

                    unsigned int get_attr1() const {
                        return m_attr1;
                    }

                    unsigned int get_attr2() const {
                        return m_attr2;
                    }

                    virtual void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num)override;
                    void mosaic_state_updated()override;
                    void on_visible_state_changed(bool hidden)override;
                    void resume_animation() override {}
                    void stop_animation(bool pause) override {}
                    void toggle_blending(bool enable_blending, bool bottom_layer)override;
                    void update_affine_state(core::gfx::AffineTransformation affine_transformation,
                                             bool new_transformation)override;

                    virtual void input(core::InputEvent input_event)override {}

                    void update(unsigned char cycle_time)override {
                        update_animation();
                    }

                    virtual void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                            const core::gfx::SpriteSize size, const unsigned int tile_id) = 0;
                    virtual void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                            const unsigned short *palette, const unsigned int palette_len,
                                            const core::gfx::SpriteSize size, const unsigned int tile_id) = 0;
                private:
                    bool m_blended = false;
                    bool m_is_4bpp;
                    unsigned int m_last_obj_attr_num;
                    bool m_mosaic = false;
                    core::gfx::SpriteSize m_sprite_size;

                    OBJ_AFFINE m_affine_base;
                    OBJ_AFFINE m_affine_current;

                    unsigned int m_attr0;
                    unsigned int m_attr1;
                    unsigned int m_attr2;
            };
        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
