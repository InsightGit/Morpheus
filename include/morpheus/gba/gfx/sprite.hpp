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
                    /// Constructs a Sprite. Note: This is an abstract class
                    /// and thus shouldn't be constructed manually. Construct
                    /// either a Sprite4Bpp or a Sprite8Bpp instead, both of
                    /// which inherit this class. For more information about
                    /// this constructor, see the parent
                    /// morpheus::core::gfx::SpriteBase's constructor.
                    /// \param affine Whether this Sprite is affine or not
                    /// \param is_4bpp Whether this Sprite is 4bpp (true) or
                    /// 8bpp (false).
                    /// \param blending_controller The
                    /// GbaBlendingController to use
                    /// \param mosaic_controller The GbaMosaicController to use
                    explicit Sprite(const bool affine, const bool is_4bpp, GbaBlendingController *blending_controller,
                                    GbaMosaicController *mosaic_controller) :
                                    core::gfx::SpriteBase(affine, blending_controller, mosaic_controller) {
                        m_is_4bpp = is_4bpp;

                        if(is_affine()) {
                            obj_aff_identity(&m_affine_current);

                            update_affine_state(core::gfx::AffineTransformation::Identity, true);
                        }
                    }

                    /// Constructs a Sprite with a predetermined tile and palette id.
                    /// Note: This is an abstract class
                    /// and thus shouldn't be constructed manually. Construct
                    /// either a Sprite4Bpp or a Sprite8Bpp instead, both of
                    /// which inherit this class. For more information about
                    /// this constructor, see the parent
                    /// morpheus::core::gfx::SpriteBase's constructor.
                    /// \param affine Whether this Sprite is affine or not
                    /// \param is_4bpp Whether this Sprite is 4bpp (true) or
                    /// 8bpp (false).
                    /// \param palette_id The predetermined [0-16] color palette id
                    /// to use
                    /// \param tile_id The predetermined [0-1024] GBA-specific
                    /// tile id to use.
                    /// \param blending_controller The
                    /// GbaBlendingController to use
                    /// \param mosaic_controller The GbaMosaicController to use
                    explicit Sprite(const bool affine, const bool is_4bpp, const unsigned short palette_id,
                                    const unsigned short tile_id, core::gfx::SpriteSize size,
                                    GbaBlendingController *blending_controller,
                                    GbaMosaicController *mosaic_controller) : Sprite(affine, is_4bpp,
                                                                                     blending_controller,
                                                                                     mosaic_controller) {
                        build_attr2(palette_id, tile_id);
                        set_sprite_size(size);
                    }

                    /// Destructs a Sprite
                    ~Sprite() override;

                    /// Builds an Sprite's internal ATTR2, setting its tile_id
                    /// and palette_id.
                    /// \param palette_id The [0-16] color palette id to use
                    /// \param tile_id The [0-1024] GBA-specific tile id to use.
                    void build_attr2(const unsigned short palette_id, const unsigned short tile_id) {
                        m_attr2 = ATTR2_BUILD(tile_id, palette_id, 0);
                    }

                    /// \return The [0-16] color palette id this Sprite is
                    /// currently using.
                    unsigned int get_palette_id() const {
                        return (m_attr2 & ATTR2_PALBANK_MASK) >> ATTR2_PALBANK_SHIFT;
                    }

                    core::gfx::SpriteSize get_sprite_size() const override {
                        return m_sprite_size;
                    }

                    /// \return The [0-1024] GBA-specific tile id this Sprite
                    /// is currently using.
                    unsigned int get_tile_id() const {
                        return (m_attr2 & ATTR2_ID_MASK) >> ATTR2_ID_SHIFT;
                    }

                    bool is_blended() const override {
                        return m_blended;
                    }

                    /// Loads grit-generated Sprite Object/OBJ data into this
                    /// Sprite. Internally calls second array_load function.
                    /// \param tile_array The grit-generated tile graphics
                    /// buffer to load
                    /// \param tile_array_len The length of the grit-generated
                    /// tile graphics buffer
                    /// \param palette The grit-generated color palette buffer
                    /// to load
                    /// \param palette_len The length of the grit-generated
                    /// color palette buffer to load
                    /// \param size The SpriteSize to be set on this Sprite
                    /// \param tile_id The GBA-specific tile_id to load the
                    /// tile graphics buffer into
                    void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const unsigned short *palette, const unsigned int palette_len,
                                         const core::gfx::SpriteSize size, const unsigned int tile_id);

                    /// Loads grit-generated Sprite Object/OBJ data into this
                    /// Sprite without a palette. This is useful when the
                    /// desired palette has already been loaded into the sprite
                    /// palette by another Sprite. Internally calls first
                    /// array_load function.
                    /// \param tile_array The grit-generated tile graphics
                    /// buffer to load
                    /// \param tile_array_len The length of the grit-generated
                    /// tile graphics buffer
                    /// \param size The SpriteSize to be set on this Sprite
                    /// \param tile_id The GBA-specific tile_id to load the
                    /// tile graphics buffer into
                    void load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                         const core::gfx::SpriteSize size, const unsigned int tile_id);

                    void set_sprite_size(core::gfx::SpriteSize size)override;
                protected:
                    /// \return The OAM ATTR0 of this Sprite used by tonc.
                    unsigned int get_attr0() const {
                        return m_attr0;
                    }

                    /// \return The OAM ATTR1 of this Sprite used by tonc.
                    unsigned int get_attr1() const {
                        return m_attr1;
                    }

                    /// \return The OAM ATTR2 of this Sprite used by tonc.
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

                    virtual void input(core::InputEvent input_event) override {}

                    void update(unsigned char cycle_time) override {
                        update_animation();
                    }

                    /// Pure virtual function which loads grit-generated tile
                    /// graphics data without a palette in a BPP-specific
                    /// way.
                    /// \param tile_array The grit-generated tile graphics
                    /// buffer to load
                    /// \param tile_array_len The length of the grit-generated
                    /// tile graphics buffer
                    /// \param size The SpriteSize to be set on this Sprite
                    /// \param tile_id The GBA-specific tile_id to load the
                    /// tile graphics buffer into
                    virtual void array_load(const unsigned short *tile_array, const unsigned int tile_array_len,
                                            const core::gfx::SpriteSize size, const unsigned int tile_id) = 0;

                    /// Pure virtual function which loads grit-generated tile
                    /// graphics data and a grit-generated palette in a
                    /// BPP-specific way.
                    /// \param tile_array The grit-generated tile graphics
                    /// buffer to load
                    /// \param tile_array_len The length of the grit-generated
                    /// tile graphics buffer
                    /// \param palette The grit-generated color palette buffer
                    /// to load
                    /// \param palette_len The length of the grit-generated
                    /// color palette buffer to load
                    /// \param size The SpriteSize to be set on this Sprite
                    /// \param tile_id The GBA-specific tile_id to load the
                    /// tile graphics buffer into
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


            /// \class morpheus::gba::gfx::Sprite
            /// An abstract class which implements much
            /// morpheus::core::gfx::SpriteBase functionality while abstracting
            /// specific Sprite loading onto Sprite4Bpp and Sprite8Bpp which
            /// inherit this class. As a result, when constructing sprites for
            /// the GBA, use those classes instead. For more info about this
            /// class, consult the parent morpheus::core::gfx::SpriteBase class.

        }
    }
}



#endif //MORPHEUS_SPRITE_HPP
