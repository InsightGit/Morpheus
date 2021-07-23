//
// Created by bobby on 16/12/2020.
//

#ifndef MORPHEUS_NDS_TEST_SPRITE_HPP
#define MORPHEUS_NDS_TEST_SPRITE_HPP

#include <nds.h>

#include <morpheus/core/core.hpp>

#include <morpheus/nds/nds_controllers.hpp>
#include <morpheus/nds/gfx/nds_animation_frame.hpp>

#pragma GCC diagnostic ignored "-Wunused-variable"

namespace morpheus {
    namespace nds {
        namespace gfx {
            enum class OamStatus {
                DISABLED, ///< The OAM is disabled
                ENABLED, ///< The OAM is enabled and in
                         ///< single palette mode.
                ENABLED_EXTENDED, ///< The OAM is enabled and in
                                  ///< extended palette mode of
                                  ///< 16 color palettes of 256 colors.
                ENABLED_BITMAP, ///< The OAM is enabled and in bitmap mode.
                                ///< (Unsupported)
                ENABLED_EXTENDED_BITMAP ///< The OAM is enabled and in
                                        ///< extended bitmap mode. (Unsupported)
            };

            /// \enum morpheus::nds::gfx::OamStatus
            /// Current NDS-specifc status of the OAM.

            enum class ExtendedPaletteStatus {
                NEEDED, ///< This DS Sprite needs extended palettes to be
                        ///< enabled
                NOTNEEDED, ///< This DS Sprite does not need extended palettes
                           ///< to be enabled or disabled
                NEEDOFF ///< This DS Sprite needs extended palettes to be
                        ///< disabled.
            };

            /// \enum morpheus::nds::gfx::ExtendedPaletteStatus
            /// Extended palette status of a Nintendo DS Sprite.

            static OamStatus OAM_STATUS = OamStatus::DISABLED;

            class Sprite : public core::gfx::SpriteBase {
            public:
                /// Constructs a Sprite. Note: This is an abstract class
                /// and thus shouldn't be constructed manually. Construct
                /// either a Sprite4Bpp or a Sprite8Bpp instead, both of
                /// which inherit this class. For more information about
                /// this constructor, see the parent
                /// morpheus::core::gfx::SpriteBase's constructor.
                /// \param affine Whether this Sprite is affine or not.
                /// \param use_sub_display Whether this Sprite will use the
                /// bottom sub display (true) or the top main display (false).
                /// \param blending_controller The GbaBlendingController to use
                /// \param mosaic_controller The GbaMosaicController to use
                /// \param sprite_mapping The libnds SpriteMapping that this
                /// Sprite should use
                /// \param external_palette The ExtendedPaletteStatus of this
                /// Sprite
                explicit Sprite(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                NdsMosaicController *mosaic_controller, SpriteMapping sprite_mapping,
                                ExtendedPaletteStatus external_palette);

                /// Constructs a Sprite with a predetermined libnds graphics
                /// pointer and morpheus::core::gfx::SpriteSize.
                /// Note: This is an abstract class
                /// and thus shouldn't be constructed manually. Construct
                /// either a Sprite4Bpp or a Sprite8Bpp instead, both of
                /// which inherit this class. For more information about
                /// this constructor, see the parent
                /// morpheus::core::gfx::SpriteBase's constructor.
                /// \param affine Whether this Sprite is affine or not.
                /// \param use_sub_display Whether this Sprite will use the
                /// bottom sub display (true) or the top main display (false).
                /// \param blending_controller The GbaBlendingController to use
                /// \param mosaic_controller The GbaMosaicController to use
                /// \param sprite_mapping The libnds SpriteMapping that this
                /// Sprite should use
                /// \param external_palette The ExtendedPaletteStatus of this
                /// Sprite
                /// \param nds_oam_address The predetermined libnds OAM sprite
                /// graphics pointer to use
                /// \param sprite_size The
                /// predetermined morpheus::core::gfx::SpriteSize to use.
                explicit Sprite(bool affine, bool use_sub_display, NdsBlendingController *blending_controller,
                                NdsMosaicController *mosaic_controller,
                                SpriteMapping sprite_mapping, ExtendedPaletteStatus external_palette,
                                unsigned short *nds_oam_address, const morpheus::core::gfx::SpriteSize sprite_size);

                /// Destructs the Sprite object.
                ~Sprite() override;

                /// \param color_format The libnds SpriteColorFormat of the
                /// Sprite to be created.
                /// \param size The SpriteSize of the graphics pointer to
                /// create
                /// \return A created libnds graphics pointer without
                /// associating it with this Sprite. Useful for Sprite
                /// animation.
                uint16_t *create_gfx_pointer(const SpriteColorFormat color_format,
                                             const morpheus::core::gfx::SpriteSize size);

                /// Prevents this Sprite's graphics pointer from being freed
                /// after this Sprite is destructed or set_gfx_pointer() is
                /// called. Useful for Sprite animation.
                void disable_gfx_pointer_frees() {
                    m_do_not_free_gfx_pointer = false;
                }

                /// Allows this Sprite's graphics pointer to be freed after this
                /// Sprite is destructed or set_gfx_pointer() is called.
                void enable_gfx_pointer_frees() {
                    m_do_not_free_gfx_pointer = true;
                }

                /// \return The current libnds graphics pointer being used by
                /// this SpriteBase.
                uint16_t *get_gfx_pointer() const {
                    return m_gfx_pointer;
                }

                /// \return The current [0-16] palette id being used by this
                /// SpriteBase.
                unsigned int get_palette_id() const {
                    return m_palette_id;
                }

                core::gfx::SpriteSize get_sprite_size() const override {
                    return m_sprite_size;
                }

                /// Sets the libnds graphics pointer of this Sprite. Useful
                /// for Sprite animation.
                /// \param pointer The libnds graphics pointer to be set.
                void set_gfx_pointer(uint16_t *pointer) {
                    if(m_gfx_pointer != nullptr && m_do_not_free_gfx_pointer) {
                        //std::cout << "freeing gfx\n";

                        oamFreeGfx(m_current_oam, m_gfx_pointer);
                    }

                    m_gfx_pointer = pointer;
                }

                /// Sets the [0-16] palette id for this SpriteBase to use.
                /// \param palette_id The [0-16] palette id to set.
                void set_palette_id(const unsigned int palette_id) {
                    m_palette_id = palette_id;
                }

                bool is_blended() const override {
                    return m_blended;
                }

                // this morpheus::core::gfx::SpriteSize is morpheus'
                void set_sprite_size(morpheus::core::gfx::SpriteSize size)override;

                // Extended palette load functions
                /// Pure virtual function which loads grit-generated tile
                /// graphics data with a [0-16] palette id in a BPP-specific
                /// way.
                /// \param tile_array The grit-generated tile graphics
                /// buffer to load
                /// \param tile_array_len The length of the grit-generated
                /// tile graphics buffer
                /// \param palette_id The [0-16] palette id to be set on this
                /// Sprite
                /// \param size The SpriteSize to be set on this Sprite
                /// \return Whether the grit-generated Sprite (or Object/OBJ)
                /// data was successfully loaded into this Sprite
                virtual bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                             const unsigned int palette_id,
                                             const morpheus::core::gfx::SpriteSize size) = 0;

                /// Pure virtual function which loads grit-generated tile
                /// graphics data with a palette and a [0-16] palette id in a
                /// BPP-specific way.
                /// \param tile_array The grit-generated tile graphics
                /// buffer to load
                /// \param tile_array_len The length of the grit-generated
                /// tile graphics buffer
                /// \param palette The grit-generated color palette buffer
                /// to load
                /// \param palette_len The length of the grit-generated
                /// color palette buffer to load
                /// \param palette_id The [0-16] palette id to be set on this
                /// Sprite
                /// \param size The SpriteSize to be set on this Sprite
                /// \return Whether the grit-generated Sprite (or Object/OBJ)
                /// data was successfully loaded into this Sprite.
                virtual bool load_from_array(const unsigned short *tile_array, const unsigned int tile_array_len,
                                             const unsigned short *palette, const unsigned int palette_len,
                                             const unsigned int palette_id,
                                             const morpheus::core::gfx::SpriteSize size) = 0;
            protected:
                /// \return The libnds representation of the current OAM being
                /// used.
                OamState *get_current_oam() const {
                    return m_current_oam;
                }

                /// \return The libnds representation of this Sprite's size.
                /// This is exclusively used internally when dealing with
                /// libnds and get_sprite_size() should be used for everything
                /// else.
                SpriteSize get_nds_sprite_size() const {
                    return m_nds_sprite_size;
                }

                /// \return Whether this Sprite is in extended palette mode (true) or
                /// single palette mode (false).
                bool is_in_extended_palette_mode() const {
                    return m_extended_palette;
                }

                void toggle_blending(bool enable_blending, bool bottom_layer)override;

                void on_visible_state_changed(bool hidden) override {
                    if(m_last_used_obj_attr_num != -1) {
                        oamSetHidden(m_current_oam, m_last_used_obj_attr_num, hidden);
                    }
                }

                void mosaic_state_updated() override {}

                virtual void update(unsigned char cycle_time) override {
                    update_animation();
                }
                virtual void input(core::InputEvent input_event) override {}
            protected:
                /// Sets the last used object (or sprite) attribute number of
                /// this Sprite.
                /// \param last_used_obj_attr_num The last used object attribute
                /// number to be set
                void set_last_used_obj_attr_num(const int last_used_obj_attr_num) {
                    if(m_last_used_obj_attr_num >= 0) {
                        m_last_used_obj_attr_num = last_used_obj_attr_num;
                    }
                }

                /// \return The current NdsAnimationFrame to be used if this
                /// Sprite is animated
                NdsAnimationFrame get_current_nds_animation_frame() const {
                    return m_frames[get_current_frame()];
                }

                void resume_animation()override {}
                void stop_animation(bool pause)override {}

                virtual void update_affine_state(core::gfx::AffineTransformation affine_transformation,
                                                 bool new_transformation) override {}
            private:
                bool m_blended;
                bool m_do_not_free_gfx_pointer;
                bool m_extended_palette;
                bool m_enabled = false;
                std::vector<NdsAnimationFrame> m_frames;
                int m_last_used_obj_attr_num = -1;
                unsigned int m_palette_id;

                OamState *m_current_oam;
                uint16_t *m_gfx_pointer = nullptr;
                core::gfx::SpriteSize m_sprite_size;
                SpriteSize m_nds_sprite_size;
            };


            /// \class morpheus::nds::gfx::Sprite
            /// An abstract class which implements much
            /// morpheus::core::gfx::SpriteBase functionality while abstracting
            /// specific Sprite loading onto Sprite4Bpp and Sprite8Bpp which
            /// inherit this class. As a result, when constructing sprites for
            /// the DS, use those classes instead. For more info about this
            /// class, consult the parent morpheus::core::gfx::SpriteBase class.
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_BASE_HPP
