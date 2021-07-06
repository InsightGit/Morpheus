//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP

#include <algorithm>
#include <set>

#include <morpheus/core/main_loop.hpp>
#include "vector_2.hpp"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class TiledBackgroundSize {
                BG_32x32, ///< 32x32 (256 px by 256 px) regular tiled
                          ///< background
                BG_64x32, ///< 64x32 (512 px by 256 px) regular tiled
                          ///< background
                BG_32x64, ///< 32x64 (256 px by 512 px) regular tiled
                          ///< background
                BG_64x64, ///< 64x64 (512 px by 512 px) regular tiled
                          ///< background

                BG_AFFINE_16x16, ///< 16x16 (128 px by 128 px) affine tiled
                                 ///< background
                BG_AFFINE_32x32, ///< 32x32 (256 px by 256 px) affine tiled
                                 ///< background
                BG_AFFINE_64x64, ///< 64x64 (512 px by 512 px) affine tiled
                                 ///< background
                BG_AFFINE_128x128 ///< 128x128 (1024 px by 1024 px) affine tiled
                                  ///< background
            };


            /// \enum morpheus::core::gfx::TiledBackgroundSize
            /// (x, y) size of a regular or affine TiledBackgroundBase and in
            /// 8x8 TILES not pixels. (affine TiledBackgroundBase support
            /// still WIP).

            enum class BitUnpacking {
                NONE, ///< No bit unpacking needed
                BPP_1_TO_4, ///< Bit unpacking from 1bpp tiles to 4bpp tiles
                BPP_1_TO_8, ///< Bit unpacking from 1bpp tiles to 8bpp tiles
                BPP_4_TO_8, ///< Bit unpacking from 4bpp tiles to 8bpp tiles
            };


            /// \enum morpheus::core::gfx::BitUnpacking
            /// Supported bit unpacking operations to do on graphical 1bpp,
            /// 4bpp, and 8bpp tiles to display on 4bpp and 8bpp backgrounds.

            class TiledBackgroundBase {
            public:
                /// Constructs a regular or affine (WIP) TiledBackgroundBase
                /// with a certain background number,
                /// blending and mosaic controllers,
                /// cbb (or tile memory offset), sbb (or tilemap memory offset),
                /// and whether to use individual TileOverrides or a completely
                /// separate read-write copy of the tilemap to update specific
                /// tiles during run-time. If the BlendingController and/or the
                /// MosaicController passed through are nullptr, this
                /// TiledBackgroundBase will not be able to modify its mosaic
                /// and/or blending states.
                /// \param affine Whether this TiledBackgroundBase should be
                /// affine
                /// \param background_num Which background number this
                /// TiledBackgroundBase should use
                /// \param blending_controller Which blending controller this
                /// TiledBackgroundBase is affected by
                /// \param mosaic_controller Which mosaic controller this
                /// TiledBackgroundBase is affected by
                /// \param cbb_num The cbb or tile memory offset for this
                /// TiledBackgroundBase to use
                /// \param sbb_num The sbb or tile memory offset for this
                /// TiledBackgroundBase to use
                /// \param use_tile_overrides Whether to use individual
                /// TileOverrides (true) or a completely separate
                /// read-write copy of the tilemap (false)
                /// to update specific tiles during run-time.
                /// Unless you are stressed for WRAM and/or do not need to
                /// update this background, it is recommended to set this to
                /// false for performance reasons.
                TiledBackgroundBase(bool affine, unsigned int background_num, BlendingController *blending_controller,
                                    MosaicController *mosaic_controller, unsigned int cbb_num, unsigned int sbb_num,
                                    bool use_tile_overrides);

                /// Destructs a TiledBackgroundBase
                virtual ~TiledBackgroundBase() = default;


                /// Loads grit-generated graphical tile data, palette data, and
                /// tilemap data (usually generated by the bintileconvert build
                /// tool (buildtools/bintileconvert/bintileconvert.py)).
                /// Optionally can unpack a lower bpp tile graphics into a 4bpp
                /// or 8bpp background.
                /// \param tiles The buffer of the grit-generated graphical tile
                /// data to load
                /// \param tiles_len The length of the graphical tile data
                /// buffer to load in bytes
                /// \param palette The buffer of the grit-generated palette data
                /// to load
                /// \param pal_len The length of the palette data buffer to load
                /// in bytes
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The size of the background being loaded
                /// \param unpacking_needed Whether bit unpacking is needed on
                /// graphical tile data and if so what kind
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     const TiledBackgroundSize size,
                                     const BitUnpacking unpacking_needed = BitUnpacking::NONE) {
                    update_tilemap_vars(tile_map, tile_map_len, size);

                    array_load(tiles, tiles_len, palette, pal_len, tile_map, tile_map_len, size, unpacking_needed);
                }

                /// Similar to previous load_from_array function
                /// except new palette data is not loaded. This is useful when
                /// another TiledBackground loaded a common color palette that
                /// both backgrounds share already into palette VRAM.
                /// \param tiles The buffer of the grit-generated graphical tile
                /// data to load
                /// \param tiles_len The length of the graphical tile data
                /// buffer to load in bytes
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The size of the background being loaded
                /// \param unpacking_needed Whether bit unpacking is needed on
                /// graphical tile data and if so what kind
                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     const TiledBackgroundSize size,
                                     const BitUnpacking unpacking_needed = BitUnpacking::NONE) {
                    update_tilemap_vars(tile_map, tile_map_len, size);

                    array_load(tiles, tiles_len, tile_map, tile_map_len, size, unpacking_needed);
                }

                /// Similar to previous load_from_array function
                /// except new tile data is also not loaded. This is useful when
                /// another TiledBackground loaded a common color palette and
                /// tiles that both backgrounds share already into palette VRAM
                /// and in the same SBB.
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The size of the background being loaded
                /// \param unpacking_needed Whether bit unpacking is needed on
                /// graphical tile data and if so what kind
                void load_from_array(const unsigned short *tile_map, const unsigned int tile_map_len,
                                     TiledBackgroundSize size) {
                    update_tilemap_vars(tile_map, tile_map_len, size);

                    array_load(tile_map, tile_map_len, size);
                }

                /// Disables the 2D blending graphical effect on this
                /// TiledBackgroundBase. If nullptr was passed into the
                /// blending_controller argument on this TiledBackgroundBase's
                /// construction, this function will have no effect.
                void disable_blending() {
                    if(m_blending_controller != nullptr) {
                        m_blending_controller->disable_background_blending(get_background_num());
                    }
                }

                /// Enables the 2D blending graphical effect on this
                /// TiledBackgroundBase and places it either on the bottom layer
                /// (otherwise known as the 2nd layer or Layer B) or the top
                /// layer (otherwise known as the 1st layer or Layer A). If
                /// nullptr was passed into the blending_controller
                /// argument on this TiledBackgroundBase's
                /// construction, this function will have no effect.
                /// \param bottom Whether to enable blending on the bottom layer
                /// (true) or the top layer (false)
                void enable_blending(bool bottom) {
                    // insures both top and bottom blending bits aren't set at the same time
                    if(m_blending_controller != nullptr) {
                        disable_blending();

                        m_blending_controller->enable_background_blending(bottom, get_background_num());
                    }
                }

                /// \return The affine index [0-32] that this affine
                /// TiledBackgroundBase is using for its affine transformation.
                /// If this TiledBackgroundBase is not affine, the value
                /// returned will be meaningless.
                unsigned int get_affine_index() const {
                    if(m_affine) {
                        return m_affine_index;
                    } else {
                        return 32u;
                    }
                }

                /// \return This TiledBackgroundBase's background number.
                unsigned int get_background_num() const {
                    return m_background_num;
                }

                /// \return This TiledBackgroundBase's cbb or tile memory offset
                /// number. Usually inclusively between 0 and 3.
                unsigned int get_cbb_num() const {
                    return m_cbb_num;
                }

                /// \return The mosaic levels of all TiledBackgroundBases
                /// with mosaic enabled. If nullptr was passed into the
                /// mosaic_controller argument on this TiledBackgroundBase's
                /// construction, the returned Vector2 will be meaningless.
                Vector2 get_mosaic_levels() const {
                    if(m_mosaic_controller != nullptr) {
                        return m_mosaic_controller->get_background_mosaic_levels();
                    } else {
                        return Vector2(0, 0);
                    }
                }

                /// \return The fixed-point rotation of this affine
                /// TiledBackgroundBase. If this TiledBackgroundBase is not
                /// affine, the returned value will be meaningless.
                int get_rotation() const {
                    return m_rotation;
                }

                /// \return The fixed-point scale of this affine
                /// TiledBackgroundBase. If this TiledBackgroundBase is not
                /// affine, the returned Vector2 will be meaningless.
                Vector2 get_scale() const {
                    return m_scale;
                }

                /// \return The corresponding tilemap position (in 8x8 TILES)
                /// for a current screen position (in PIXELS) based off of this
                /// current TiledBackgroundBase's scroll Vector2.
                /// \param screen_position The screen position to convert.
                Vector2 get_tile_map_position_at_screen_position(Vector2 screen_position) const {
                    screen_position = screen_position + m_scroll_position;

                    return morpheus::core::gfx::Vector2(screen_position.get_x() / 8, screen_position.get_y() / 8);
                }

                /// \return This TiledBackgroundBase's sbb or tilemap memory offset
                /// number. Usually inclusively between 0 and 31.
                unsigned int get_sbb_num() const {
                    return m_sbb_num;
                }

                /// \return This TiledBackgroundBase's current tilemap scroll
                /// Vector2 in PIXELS.
                Vector2 get_scroll() const {
                    return m_scroll_position;
                }

                /// \return Whether this TiledBackgroundBase is affine or not.
                bool is_affine() const {
                    return m_affine;
                }

                /// \return Whether this TiledBackgroundBase has mosaic enabled
                /// on it or not.
                bool is_mosaic() const {
                    return m_mosaic;
                }

                /// Sets the affine index [0-32] that this affine
                /// TiledBackgroundBase is using for its affine transformation.
                /// If this TiledBackgroundBase is not affine, this function
                /// will have no effect.
                /// \param affine_index The new affine index to be set on this
                /// TiledBackgroundBase
                void set_affine_index(const unsigned int affine_index) {
                    if(m_affine) {
                        m_affine_index = std::min(31u, affine_index);
                    }
                }

                /// Sets the mosaic levels of all TiledBackgroundBases
                /// with mosaic enabled. If nullptr was passed into the
                /// mosaic_controller argument on this TiledBackgroundBase's
                /// construction, this function will have no effect.
                /// \param mosaic_levels The new mosaic levels to set on this
                /// TiledBackgroundBase
                void set_mosaic_levels(morpheus::core::gfx::Vector2 mosaic_levels) {
                    if(m_mosaic_controller != nullptr) {
                        m_mosaic_controller->set_background_mosaic_levels(mosaic_levels);
                    }
                }

                /// Sets the fixed-point rotation of this affine
                /// TiledBackgroundBase. If this TiledBackgroundBase is not
                /// affine, this function will have no effect.
                /// \param rotation The new fixed-point rotation to be set
                /// on this TiledBackgroundBase
                void set_rotation(const int rotation) {
                    if(m_affine) {
                        m_rotation = rotation;

                        affine_state_updated();
                    }
                }

                /// Sets the fixed-point scale of this affine
                /// TiledBackgroundBase. If this TiledBackgroundBase is not
                /// affine, this function will have no effect.
                /// \param scale The new fixed-point scale to be set
                /// on this TiledBackgroundBase
                void set_scale(const core::gfx::Vector2 scale) {
                    if(m_affine) {
                        m_scale = scale;

                        affine_state_updated();
                    }
                }

                /// Sets this TiledBackgroundBase's tilemap scroll in
                /// PIXELS (NOT tiles).
                /// \param scroll_position The new tilemap scroll to set on
                /// this TiledBackgroundBase.
                void set_scroll(Vector2 scroll_position) {
                    m_scroll_position = scroll_position % ((get_size_vector() * Vector2(8, 8)) - TILE_SCREEN_SIZE);

                    update_scroll();
                }

                /// Toggles mosaic's enabled state on this TiledBackgroundBase.
                void toggle_mosaic() {
                    m_mosaic = !m_mosaic;

                    mosaic_state_updated();
                }

                /// Gets a tile id (in terms of the CBB's 8x8 tile graphics)
                /// for a given tilemap index that can be retrieved using
                /// TiledBackgroundBase::get_tile_index_at_position().
                /// \param tile_index The tilemap index to get the tile id at.
                /// \return The corresponding tile id for the tilemap index.
                int get_tile_id_at_index(const unsigned int tile_index) const;

                /// Gets a tile id (in terms of the CBB's 8x8 tile graphics)
                /// for a given tilemap position in either 8x8 tiles or pixels.
                /// \param position The tilemap position for getting the tile
                /// id.
                /// \param with_scrolling Whether to add this
                /// TiledBackgroundBase's scrolling to the tilemap position
                /// (true) or not (false)
                /// \param tiled_position Whether the position given is in terms
                /// of 8x8 tiles (true) or pixels (false).
                /// \return The corresponding tile id for the position passed
                /// in.
                int get_tile_id_at_position(const Vector2 position, const bool with_scrolling = true,
                                            const bool tiled_position = false) const;

                /// \return The tilemap index for a given tilemap position in
                /// either 8x8 tiles or pixels.
                /// \param position The tilemap position for getting the tilemap
                /// index.
                /// \param with_scrolling
                /// \param tiled_position
                int get_tile_index_at_position(const Vector2 position, const bool with_scrolling = true,
                                               const bool tiled_position = false) const;

                /// \return The screen position corresponding for a given
                /// tilemap position in either 8x8 tiles or pixels.
                /// \param position The tilemap position for getting the
                /// screen position
                /// \param with_scrolling Whether or not to include this
                /// TiledBackgroundBase's scrolling in computing the tilemap position
                /// \param tiled_position Whether the tilemap position given is
                /// tiled in 8x8 tiles or not
                Vector2 get_tile_position_at_screen_position(const Vector2 position, const bool with_scrolling = true,
                                                             const bool tiled_position = false) const;

                /// Sets a specific tile id at a specific tilemap index (in
                /// terms of the CBB's 8x8 tile graphics).
                /// \param tile_index The tilemap index to set the tile id at
                /// \param tile_id The new tile id to be set
                /// \return Whether the tilemap id was successfully set or not
                bool set_tile_id_at_index(const unsigned int tile_index, const unsigned int tile_id);

                /// Sets a specific tile id at a specific tilemap position in
                /// either 8x8 tiles or pixels.
                /// \param position The tilemap position to set the tile id at
                /// \param tile_id The new tile id to be set
                /// \param with_scrolling Whether or not to include this
                /// TiledBackgroundBase's scrolling in computing the tilemap index
                /// \param tiled_position Whether the tilemap position given is
                /// tiled in 8x8 tiles or not
                /// \return Whether the tilemap id was successfully set or not
                bool set_tile_id_at_position(const Vector2 position, const unsigned int tile_id,
                                             const bool with_scrolling = true, const bool tiled_position = false);

                /// \return This TiledBackgroundBase's global priority [0-3].
                virtual unsigned int get_priority() const = 0;
                /*virtual bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                               const unsigned int pal_offset = 256) = 0;*/

                /// Sets this TiledBackgroundBase's global priority, which can
                /// be inclusively between 0 and 3 and will place it behind
                /// SpriteBases and TiledBackgroundBases that have lower
                /// priorities, and ahead of ones that have higher priorities.
                /// \param priority The [0-3] new priority to set on this
                /// TiledBackgroundBase
                virtual void set_priority(unsigned int priority) = 0;
            protected:
                /// \return The corresponding size Vector2 for the given
                /// TiledBackgroundSize enum size in this TiledBackgroundBase.
                /// If it is an unrecoginzed value within the enum,
                /// Vector2(0, 0) is returned.
                Vector2 get_tile_map_size_vector() const {
                    switch (m_tile_map_size) {
                        case TiledBackgroundSize::BG_32x32:
                            return Vector2(32, 32);
                        case TiledBackgroundSize::BG_64x32:
                            return Vector2(64, 32);
                        case TiledBackgroundSize::BG_32x64:
                            return Vector2(32, 64);
                        case TiledBackgroundSize::BG_64x64:
                            return Vector2(64, 64);
                        default:
                            return Vector2(0, 0);
                    }
                }

                /// Sets a given tile map with a given tile map size when
                /// creating a read/write copy to update instead of
                /// individual TileOverrides.
                /// \param tile_map The tile map to set
                /// \param tile_map_size The tile map size of the tile map being set.
                void set_tile_map(const unsigned short *tile_map, TiledBackgroundSize tile_map_size) {
                    m_tile_map = tile_map;
                    m_tile_map_size = tile_map_size;
                }

                /// Pure virtual function called upon a change in this
                /// TiledBackgroundBase's affine state (whether it is affine or
                /// not)
                virtual void affine_state_updated() = 0;

                /// Pure virtual function called to load grit-generated
                /// graphical tile and palette data as well as tilemap data in
                /// a platform-specific way. Optionally can unpack a lower bpp
                /// tile graphics into a 4bpp or 8bpp background. Usually
                /// called by a public load_from_array function.
                /// \param tiles The buffer of the grit-generated graphical tile
                /// data to load
                /// \param tiles_len The length of the graphical tile data
                /// buffer to load in bytes
                /// \param palette The buffer of the grit-generated palette data
                /// to load
                /// \param pal_len The length of the palette data buffer to load
                /// in bytes
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The size of the background being loaded
                /// \param unpacking_needed Whether bit unpacking is needed on
                /// graphical tile data and if so what kind
                virtual void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                        const unsigned short *palette, const unsigned int pal_len,
                                        const unsigned short *tile_map, const unsigned int tile_map_len,
                                        const TiledBackgroundSize size,
                                        const BitUnpacking unpacking_needed = BitUnpacking::NONE) = 0;

                /// Pure virtual function called to load grit-generated
                /// graphical tile data as well as tilemap data in
                /// a platform-specific way. Optionally can unpack a lower bpp
                /// tile graphics into a 4bpp or 8bpp background. Usually
                /// called by a public load_from_array function.
                /// \param tiles The buffer of the grit-generated graphical tile
                /// data to load
                /// \param tiles_len The length of the graphical tile data
                /// buffer to load in bytes
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The size of the background being loaded
                /// \param unpacking_needed Whether bit unpacking is needed on
                /// graphical tile data and if so what kind
                virtual void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                        const unsigned short *tile_map, const unsigned int tile_map_len,
                                        const TiledBackgroundSize size,
                                        const BitUnpacking unpacking_needed = BitUnpacking::NONE) = 0;


                /// Pure virtual function called to load tilemap data in
                /// a platform-specific way. Usually called by a public
                /// load_from_array function.
                /// \param tile_map The buffer of the tilemap to load
                /// \param tile_map_len The length of the tilemap buffer to load
                /// \param size The size of the background being loaded
                virtual void array_load(const unsigned short *tile_map, const unsigned int tile_map_len,
                                        const TiledBackgroundSize size) = 0;

                /// Pure virtual function called upon a change in this
                /// TiledBackgroundBase's mosaic state (whether it has the
                /// graphical mosaic effect activated or not)
                virtual void mosaic_state_updated() = 0;

                /// Pure virtual function that overrides a given tilemap index's
                /// current tile id value with an updated one after a
                /// set_tile_id function was called for
                /// the specific tilemap index in a platform specific way.
                /// \param tile_index The tilemap index to override the tile id
                /// \param tile_id The new tile id to be put in the given
                /// tilemap index
                virtual void override_map_tile(const unsigned int tile_index, const unsigned short tile_id) = 0;

                /// Pure virtual function which updates the scroll for this
                /// TiledBackgroundBase in a platform specific way after
                /// TiledBackgroundBase::set_scroll was called.
                virtual void update_scroll() = 0;
            private:
                struct TileOverride {
                    unsigned int tile_index;
                    unsigned int tile_id;
                };

                const static Vector2 TILE_SCREEN_SIZE;

                Vector2 get_size_vector() const {
                    switch(m_tile_map_size) {
                        case TiledBackgroundSize::BG_32x32:
                            return Vector2(32, 32);
                        case TiledBackgroundSize::BG_64x32:
                            return Vector2(64, 32);
                        case TiledBackgroundSize::BG_32x64:
                            return Vector2(32, 64);
                        case TiledBackgroundSize::BG_64x64:
                            return Vector2(64, 64);
                        case TiledBackgroundSize::BG_AFFINE_16x16:
                            return Vector2(16, 16);
                        case TiledBackgroundSize::BG_AFFINE_32x32:
                            return Vector2(32, 32);
                        case TiledBackgroundSize::BG_AFFINE_64x64:
                            return Vector2(64, 64);
                        case TiledBackgroundSize::BG_AFFINE_128x128:
                            return Vector2(128, 128);
                    }

                    return Vector2(0, 0);
                }

                void update_tilemap_vars(const unsigned short *tile_map, const unsigned int tile_map_len,
                                         morpheus::core::gfx::TiledBackgroundSize size);

                bool m_affine;
                unsigned int m_affine_index;
                gfx::BlendingController *m_blending_controller;
                unsigned int m_background_num;
                unsigned int m_cbb_num;
                bool m_mosaic = false;
                gfx::MosaicController *m_mosaic_controller;
                std::vector<TileOverride> m_past_tile_overrides;
                int m_rotation = 0;
                Vector2 m_scale = Vector2(1 << 8, 1 << 8);
                Vector2 m_scroll_position;
                unsigned int m_sbb_num;
                const unsigned short *m_tile_map;
                std::vector<unsigned short> m_tile_map_rw_copy;
                std::vector<TileOverride> m_tile_overrides;
                TiledBackgroundSize m_tile_map_size;
                bool m_use_tile_overrides;
            };


            /// \class morpheus::core::gfx::TiledBackgroundBase
            /// A tiled background class that can either be in regular
            /// (or "text") mode or (currently WIP) affine (or "rotscale") mode.
            /// Also supports several hardware graphical effects like mosaic
            /// (making the background look more "blocky"), blending (making the
            /// background either fade to white or black, or making it look more
            /// transparent), and windowing (constraining the displayed
            /// background in a specific "window" rectangle). For an example of
            /// this class being used, see the Graphical Effects Test
            /// (tests/gfx_effects_test/gfx_effects_test.cpp) and the Tileset
            /// Test (tests/tileset_test/tileset_test.cpp).
        }
    }
}

extern "C" {
    struct asm_BitUnPackOptions {
        unsigned short source_len; ///< The length of the tile graphics buffer
                                   ///< being unpacked
        unsigned char source_bit_width; ///< The original BPP depth/width of
                                        ///< the tile graphics buffer being
                                        ///< unpacked
        unsigned char dest_bit_width; ///< The BPP depth/width of the tile
                                      ///< graphics buffer to be unpacked to
        unsigned int offset_plus_zero_data_flag; ///< The offset of the non-zero
                                                 ///< data to be loaded in as
                                                 ///< well as the "zero data"
                                                 ///< flag (on the 31st bit)
                                                 ///< which applies the offset
                                                 ///< to zeroed data as well
    };


    /// \struct asm_BitUnPackOptions
    /// A struct that serves to provide the arguments for the BitUnPack BIOS
    /// call.

    // defined in asm/tiled_background_base.s
    /// Calls the BitUnpack BIOS call to unpack a lower bpp
    /// tile graphics into a 4bpp or 8bpp background
    /// \param src The tile graphics buffer to unpack
    /// \param dest Where to unpack the tile graphics
    /// \param unpack_options The asm_BitUnpackOptions to use
    extern void asm_BitUnPack(const void *src, void *dest, asm_BitUnPackOptions *unpack_options);
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP