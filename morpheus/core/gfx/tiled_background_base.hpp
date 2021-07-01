//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
#define MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP

#include <algorithm>
#include <set>

#include <core/main_loop.hpp>
#include <core/gfx/vector_2.hpp>

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

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *palette, const unsigned int pal_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     const TiledBackgroundSize size,
                                     const BitUnpacking unpacking_needed = BitUnpacking::NONE) {
                    update_tilemap_vars(tile_map, tile_map_len, size);

                    array_load(tiles, tiles_len, palette, pal_len, tile_map, tile_map_len, size, unpacking_needed);
                }

                void load_from_array(const unsigned int *tiles, const unsigned int tiles_len,
                                     const unsigned short *tile_map, const unsigned int tile_map_len,
                                     const TiledBackgroundSize size,
                                     const BitUnpacking unpacking_needed = BitUnpacking::NONE) {
                    update_tilemap_vars(tile_map, tile_map_len, size);

                    array_load(tiles, tiles_len, tile_map, tile_map_len, size, unpacking_needed);
                }

                void load_from_array(const unsigned short *tile_map, const unsigned int tile_map_len,
                                     TiledBackgroundSize size) {
                    update_tilemap_vars(tile_map, tile_map_len, size);

                    array_load(tile_map, tile_map_len, size);
                }

                void disable_blending() {
                    m_blending_controller->disable_background_blending(get_background_num());
                }

                void enable_blending(bool bottom) {
                    // insures both top and bottom blending bits aren't set at the same time
                    disable_blending();

                    m_blending_controller->enable_background_blending(bottom, get_background_num());
                }

                unsigned int get_affine_index() const {
                    if(m_affine) {
                        return m_affine_index;
                    } else {
                        return 32u;
                    }
                }

                unsigned int get_background_num() const {
                    return m_background_num;
                }

                unsigned int get_cbb_num() const {
                    return m_cbb_num;
                }

                Vector2 get_mosaic_levels() const {
                    return m_mosaic_controller->get_background_mosaic_levels();
                }

                int get_rotation() const {
                    return m_rotation;
                }

                Vector2 get_scale() const {
                    return m_scale;
                }

                Vector2 get_tile_map_position_at_screen_position(Vector2 screen_position) const {
                    screen_position = screen_position + m_scroll_position;

                    return morpheus::core::gfx::Vector2(screen_position.get_x() / 8, screen_position.get_y() / 8);
                }

                unsigned int get_sbb_num() const {
                    return m_sbb_num;
                }

                Vector2 get_scroll() const {
                    return m_scroll_position;
                }

                bool is_affine() const {
                    return m_affine;
                }

                bool is_mosaic() const {
                    return m_mosaic;
                }

                void set_affine_index(const unsigned int affine_index) {
                    if(m_affine) {
                        m_affine_index = std::min(31u, affine_index);
                    }
                }

                void set_mosaic_levels(morpheus::core::gfx::Vector2 mosaic_levels) {
                    m_mosaic_controller->set_background_mosaic_levels(mosaic_levels);
                }

                void set_rotation(const int rotation) {
                    if(m_affine) {
                        m_rotation = rotation;

                        affine_state_updated();
                    }
                }

                void set_scale(const core::gfx::Vector2 scale) {
                    if(m_affine) {
                        m_scale = scale;

                        affine_state_updated();
                    }
                }

                void set_scroll(Vector2 scroll_position) {
                    m_scroll_position = scroll_position % ((get_size_vector() * Vector2(8, 8)) - TILE_SCREEN_SIZE);

                    update_scroll();
                }

                void toggle_mosaic() {
                    m_mosaic = !m_mosaic;

                    mosaic_state_updated();
                }

                int get_tile_id_at_index(const unsigned int tile_index) const;
                int get_tile_id_at_position(const Vector2 position, const bool with_scrolling = true,
                                            const bool tiled_position = false) const;
                int get_tile_index_at_position(const Vector2 position, const bool with_scrolling = true,
                                               const bool tiled_position = false) const;
                Vector2 get_tile_position_at_screen_position(const Vector2 position, const bool with_scrolling = true,
                                                             const bool tiled_position = false) const;
                bool set_tile_id_at_index(const unsigned int tile_index, const unsigned int tile_id);
                bool set_tile_id_at_position(const Vector2 position, const unsigned int tile_id,
                                             const bool with_scrolling = true, const bool tiled_position = false);

                virtual unsigned int get_priority() const = 0;
                /*virtual bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                               const unsigned int pal_offset = 256) = 0;*/
                virtual void set_priority(unsigned int priority) = 0;
            protected:
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

                void set_tile_map(const unsigned short *tile_map, TiledBackgroundSize tile_map_size) {
                    m_tile_map = tile_map;
                    m_tile_map_size = tile_map_size;
                }

                virtual void affine_state_updated() = 0;
                virtual void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                        const unsigned short *palette, const unsigned int pal_len,
                                        const unsigned short *tile_map, const unsigned int tile_map_len,
                                        const TiledBackgroundSize size,
                                        const BitUnpacking unpacking_needed = BitUnpacking::NONE) = 0;
                virtual void array_load(const unsigned int *tiles, const unsigned int tiles_len,
                                        const unsigned short *tile_map, const unsigned int tile_map_len,
                                        const TiledBackgroundSize size,
                                        const BitUnpacking unpacking_needed = BitUnpacking::NONE) = 0;
                virtual void array_load(const unsigned short *tile_map, const unsigned int tile_map_len,
                                        const TiledBackgroundSize size) = 0;
                virtual void mosaic_state_updated() = 0;
                virtual void override_map_tile(const unsigned int tile_index, const unsigned short tile_id) = 0;
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
        }
    }
}

extern "C" {
    struct asm_BitUnPackOptions {
        unsigned short source_len;
        unsigned char source_bit_width;
        unsigned char dest_bit_width;
        unsigned int offset_plus_zero_data_flag;
    };

    // defined in asm/tiled_background_base.s
    extern void asm_BitUnPack(const void *src, void *dest, asm_BitUnPackOptions *unpack_options);
}

#endif //MORPHEUS_GBA_TEST_TILED_BACKGROUND_BASE_HPP
