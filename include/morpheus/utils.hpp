//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_UTILS_HPP
#define MORPHEUS_GBA_TEST_UTILS_HPP

#ifdef _GBA
    #include <morpheus/gba/gba.hpp>
#elif _NDS
    #include <morpheus/nds/nds.hpp>
#else
    #error Unsupported Platform!
#endif

#pragma GCC diagnostic ignored "-Wunused-function"

namespace morpheus {
    namespace utils {
        class BackgroundTestControls : public morpheus::core::ControlReciever {
        public:
            /// Constructs a BackgroundTestControls object with a given smart
            /// pointer to a TiledBackgroundBase.
            /// \param background A smart pointer to the TiledBackgroundBase
            /// this class will control
            BackgroundTestControls(std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background) {
                m_background = background;
            }

            void input(morpheus::core::InputEvent input_event)override {
                scroll_background(input_event);
            }

            void update(unsigned char cycle_time) override {}

            /// Changes the current smart pointer to the TiledBackgroundBase
            /// that this class will control.
            /// \param new_background The smart pointer that this class will
            /// control
            void change_background(const std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> new_background) {
                m_background = new_background;
            }
        protected:
            /// Scrolls the background according to InputEvents feeded by
            /// input()
            /// \param input_event The InputEvent from input() to react to
            void scroll_background(morpheus::core::InputEvent input_event) {
                if(input_event.state == morpheus::core::InputState::DOWN ||
                   input_event.state == morpheus::core::InputState::HELD) {
                    morpheus::core::gfx::Vector2 scroll_pos = m_background->get_scroll();

                    switch(input_event.button) {
                        case morpheus::core::InputButton::DPADUP:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() - 10);
                            break;
                        case morpheus::core::InputButton::DPADLEFT:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() - 10, scroll_pos.get_y());
                            break;
                        case morpheus::core::InputButton::DPADRIGHT:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() + 10, scroll_pos.get_y());
                            break;
                        case morpheus::core::InputButton::DPADDOWN:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() + 10);
                            break;
                        default:
                            break;
                    }

                    m_background->set_scroll(scroll_pos);
                }
            }
        private:
            std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background;
        };

        /// \class morpheus::utils::BackgroundTestControls
        /// A helper class for testing background scrolling with the directional
        /// pad.


        /// Constructs the appropriate morpheus::core::gfx::AnimationFrame for
        /// the current platform.
        /// \param sprite_base The sprite for the AnimationFrame
        /// \param from_animation_frame The optional from_animation_frame
        /// argument for the AnimationFrame
        /// \return The appropriate AnimationFrame
        static morpheus::core::gfx::AnimationFrame *construct_appropriate_animation_frame(
                core::gfx::SpriteBase *sprite_base, core::gfx::AnimationFrame *from_animation_frame = nullptr) {
            #ifdef _GBA
                return new morpheus::gba::gfx::GbaAnimationFrame(
                                                    static_cast<gba::gfx::Sprite*>(sprite_base),
                                                    static_cast<gba::gfx::GbaAnimationFrame*>(from_animation_frame));
            #elif _NDS
                return new morpheus::nds::gfx::NdsAnimationFrame(
                                                     static_cast<nds::gfx::Sprite*>(sprite_base),
                                                     static_cast<nds::gfx::NdsAnimationFrame*>(from_animation_frame));
            #endif

            return nullptr;
        }

        /// Constructs the appropriate morpheus::core::MainLoop for
        /// the current platform.
        /// \param gba_save_type The appropriate GbaSaveType used if
        /// this function constructs a GbaMainLoop
        /// \return The appropriate MainLoop
        static morpheus::core::MainLoop *construct_appropriate_main_loop(
                                morpheus::core::GbaSaveType gba_save_type = morpheus::core::GbaSaveType::SRAM_32KB) {
            #ifdef _GBA
                return new morpheus::gba::GbaMainLoop(gba_save_type);
            #elif _NDS
                return new morpheus::nds::NdsMainLoop();
            #endif

            return nullptr;
        }

        /// Constructs the appropriate morpheus::core::audio::MaxModMusic for
        /// the current platform.
        /// \param sound_bank_ref_num The sound bank reference number
        /// argument of MaxModMusic
        /// \param sound_bank The sound bank pointer argument of MaxModMusic
        /// \param num_of_channels The number of audio channels to use if
        /// constructing a GbaMaxModMusic
        /// \return The appropriate MaxModMusic object
        static morpheus::core::audio::MaxModMusic *construct_appropriate_max_mod_music(
                                                                                   int sound_bank_ref_num,
                                                                                   void *sound_bank = nullptr,
                                                                                   unsigned char num_of_channels = -1) {
            #ifdef _GBA
                return new morpheus::gba::audio::GbaMaxModMusic(sound_bank_ref_num, sound_bank, num_of_channels);
            #elif _NDS
                return new morpheus::nds::audio::NdsMaxModMusic(sound_bank_ref_num, sound_bank);
            #endif

            return nullptr;
        }

        /// Constructs the appropriate morpheus::core::audio::MaxModSfx for
        /// the current platform.
        /// \param sound_bank_ref_num The sound bank reference number
        /// argument of MaxModSfx
        /// \param sound_bank The sound bank pointer argument of MaxModSfx
        /// \param num_of_channels The number of audio channels to use if
        /// constructing a GbaMaxModSfx
        /// \return The appropriate MaxModSfx object
        static morpheus::core::audio::MaxModSfx *construct_appropriate_max_mod_sfx(
                int sound_bank_ref_num,
                void *sound_bank = nullptr,
                int num_of_channels = -1) {
            #ifdef _GBA
                return new morpheus::gba::audio::GbaMaxModSfx(sound_bank_ref_num, sound_bank, num_of_channels);
            #elif _NDS
                return new morpheus::nds::audio::NdsMaxModSfx(sound_bank_ref_num, sound_bank);
            #endif

            return nullptr;
        }

        /// Constructs the appropriate 4bpp morpheus::core::gfx::SpriteBase
        /// for the current platform.
        /// \param affine The affine argument of SpriteBase
        /// \param blending_controller The blending controller argument of
        /// SpriteBase
        /// \param mosaic_controller The mosaic controller argument of
        /// SpriteBase
        /// \param nds_use_sub_display If constructing a 4bpp sprite for the DS,
        /// should this sprite be displayed on the bottom sub display (true) or
        /// the top main display (false)
        /// \return The appropriate 4bpp SpriteBase object
        static morpheus::core::gfx::SpriteBase *construct_appropriate_sprite_4bpp(
                bool affine,
                morpheus::core::gfx::BlendingController *blending_controller,
                morpheus::core::gfx::MosaicController *mosaic_controller, bool nds_use_sub_display = false) {
            #ifdef _GBA
                return new morpheus::gba::gfx::Sprite4Bpp(affine,
                        static_cast<morpheus::gba::gfx::GbaBlendingController*>(blending_controller),
                        static_cast<morpheus::gba::gfx::GbaMosaicController*>(mosaic_controller));
            #elif _NDS
                return new morpheus::nds::gfx::Sprite4Bpp(
                        affine,
                        nds_use_sub_display,
                        static_cast<morpheus::nds::gfx::NdsBlendingController*>(blending_controller),
                        static_cast<morpheus::nds::gfx::NdsMosaicController*>(mosaic_controller));
            #endif
        }

        /// Constructs the appropriate 8bpp morpheus::core::gfx::SpriteBase for
        /// the current platform.
        /// \param affine The affine argument of SpriteBase
        /// \param blending_controller The blending controller argument of
        /// SpriteBase
        /// \param mosaic_controller The mosaic controller argument of
        /// SpriteBase
        /// \param nds_use_sub_display If constructing an 8bpp sprite for the
        /// DS, should this sprite be displayed on the bottom sub display (true)
        /// or the top main display (false)
        /// \param nds_use_sub_display If constructing an 8bpp sprite for the
        /// DS, should extended palette mode be enabled or disabled
        /// \return The appropriate 4bpp SpriteBase object
        static morpheus::core::gfx::SpriteBase *construct_appropriate_sprite_8bpp(
                bool affine,
                morpheus::core::gfx::BlendingController *blending_controller,
                morpheus::core::gfx::MosaicController *mosaic_controller,
                bool nds_use_sub_display = false, bool nds_use_extended_palettes = false) {
            #ifdef _GBA
                return new morpheus::gba::gfx::Sprite8Bpp(affine,
                        static_cast<morpheus::gba::gfx::GbaBlendingController*>(blending_controller),
                        static_cast<morpheus::gba::gfx::GbaMosaicController*>(mosaic_controller));
            #elif _NDS
                return new morpheus::nds::gfx::Sprite8Bpp(
                        affine,
                        nds_use_sub_display,
                        static_cast<morpheus::nds::gfx::NdsBlendingController*>(blending_controller),
                        static_cast<morpheus::nds::gfx::NdsMosaicController*>(mosaic_controller),
                        nds_use_extended_palettes);
            #endif
        }

        /// Constructs the appropriate morpheus::core::gfx::TextBase for
        /// the current platform.
        /// \param affine The affine argument of TextBase
        /// \param background_num The background number argument of TextBase
        /// \param cbb The tile offset number argument of TextBase
        /// \param sbb The tilemap offset number argument of TextBase
        /// \param main_loop The main loop pointer argument of TextBase
        /// \param nds_use_sub_display If constructing a TextBase for the
        /// DS, should the text be displayed on the bottom sub display (true)
        /// or the top main display (false)
        /// \return The appropriate TextBase object
        static morpheus::core::gfx::TextBase *construct_appropriate_text(bool affine, unsigned int background_num,
                                                                         unsigned int cbb, unsigned int sbb,
                                                                         morpheus::core::MainLoop *main_loop,
                                                                         bool nds_use_sub_display = false) {
            #ifdef _GBA
                return new morpheus::gba::gfx::Text(affine, background_num, cbb, sbb,
                                                    static_cast<morpheus::gba::GbaMainLoop*>(main_loop));
            #elif _NDS
                return new morpheus::nds::gfx::Text(nds_use_sub_display, affine, background_num, cbb, sbb,
                                                    morpheus::nds::gfx::TextBackgroundSize::BGSIZE_32_X_32,
                                                    static_cast<morpheus::nds::NdsMainLoop*>(main_loop));
            #endif
        }

        /// Constructs the appropriate 4bpp
        /// morpheus::core::gfx::TiledBackgroundBase for the current
        /// platform.
        /// \param affine The affine argument of TiledBackgroundBase
        /// \param background_num The background number argument of
        /// TiledBackgroundBase
        /// \param blending_controller The blending controller argument of
        /// TiledBackgroundBase
        /// \param mosaic_controller The mosaic controller argument of
        /// TiledBackgroundBase
        /// \param main_loop The main loop pointer argument of
        /// TiledBackgroundBase
        /// \param cbb_num The tile offset number argument of
        /// TiledBackgroundBase
        /// \param sbb_num The tilemap offset number argument of
        /// TiledBackgroundBase
        /// \param nds_use_sub_display If constructing a TiledBackgroundBase for
        /// the DS, should the background be displayed on the bottom sub display
        /// (true) or the top main display (false)
        /// \return The appropriate 4bpp TiledBackgroundBase
        static morpheus::core::gfx::TiledBackgroundBase *construct_appropriate_tiled_background_4bpp(
                                    bool affine, unsigned int background_num,
                                    morpheus::core::gfx::BlendingController *blending_controller,
                                    morpheus::core::gfx::MosaicController *mosaic_controller,
                                    morpheus::core::MainLoop *main_loop, unsigned int cbb_num, unsigned int sbb_num,
                                    bool nds_use_sub_display = false) {
            #ifdef _GBA
                return new morpheus::gba::gfx::TiledBackground(affine, background_num,
                            static_cast<morpheus::gba::gfx::GbaBlendingController*>(blending_controller),
                            static_cast<morpheus::gba::gfx::GbaMosaicController*>(mosaic_controller),
                            static_cast<morpheus::gba::GbaMainLoop*>(main_loop), false, cbb_num, sbb_num);
            #elif _NDS
                return new morpheus::nds::gfx::TiledBackground4Bpp(
                        affine, nds_use_sub_display, background_num,
                        static_cast<morpheus::nds::gfx::NdsBlendingController*>(blending_controller),
                        static_cast<morpheus::nds::gfx::NdsMosaicController*>(mosaic_controller),
                        static_cast<morpheus::nds::NdsMainLoop*>(main_loop), cbb_num, sbb_num);
            #endif
        }

        /// Constructs the appropriate 8bpp
        /// morpheus::core::gfx::TiledBackgroundBase for the current
        /// platform.
        /// \param affine The affine argument of TiledBackgroundBase
        /// \param background_num The background number argument of
        /// TiledBackgroundBase
        /// \param blending_controller The blending controller argument of
        /// TiledBackgroundBase
        /// \param mosaic_controller The mosaic controller argument of
        /// TiledBackgroundBase
        /// \param main_loop The main loop pointer argument of
        /// TiledBackgroundBase
        /// \param cbb_num The tile offset number argument of
        /// TiledBackgroundBase
        /// \param sbb_num The tilemap offset number argument of
        /// TiledBackgroundBase
        /// \param nds_use_sub_display If constructing a TiledBackgroundBase for
        /// the DS, should the background be displayed on the bottom sub display
        /// (true) or the top main display (false)
        /// \return The appropriate 8bpp TiledBackgroundBase
        static morpheus::core::gfx::TiledBackgroundBase *construct_appropriate_tiled_background_8bpp(
                bool affine, unsigned int background_num,
                morpheus::core::gfx::BlendingController *blending_controller,
                morpheus::core::gfx::MosaicController *mosaic_controller,
                morpheus::core::MainLoop *main_loop, unsigned int cbb_num, unsigned int sbb_num,
                bool nds_use_sub_display = false) {
            #ifdef _GBA
                return new morpheus::gba::gfx::TiledBackground(affine, background_num,
                                static_cast<morpheus::gba::gfx::GbaBlendingController*>(blending_controller),
                                static_cast<morpheus::gba::gfx::GbaMosaicController*>(mosaic_controller),
                                static_cast<morpheus::gba::GbaMainLoop*>(main_loop), true, cbb_num, sbb_num);
            #elif _NDS
                return new morpheus::nds::gfx::TiledBackground8Bpp(
                        affine, nds_use_sub_display, background_num,
                        static_cast<morpheus::nds::gfx::NdsBlendingController*>(blending_controller),
                        static_cast<morpheus::nds::gfx::NdsMosaicController*>(mosaic_controller),
                        static_cast<morpheus::nds::NdsMainLoop*>(main_loop), cbb_num, sbb_num);
            #endif
        }

        /// Constructs the appropriate morpheus::core::gfx::Window for the
        /// current platform.
        /// \param window_type The window type argument of Window
        /// \param main_loop The main loop pointer argument of Window
        /// \param nds_use_sub_display If constructing a Window for
        /// the DS, should the window be on the bottom sub display
        /// (true) or the top main display (false)
        /// \return The appropriate Window object
        static morpheus::core::gfx::Window *construct_appropriate_window(const core::gfx::WindowType window_type,
                                                                         const std::shared_ptr<core::MainLoop> main_loop,
                                                                         const bool nds_use_sub_display = false) {
            #ifdef _GBA
                return new morpheus::gba::gfx::GbaWindow(window_type, main_loop);
            #elif _NDS
                return new morpheus::nds::gfx::NdsWindow(nds_use_sub_display, window_type, main_loop);
            #endif
        }

        /// Constructs a morpheus::core::gfx::StreamingBackgroundBase with
        /// sensible defaults for the current platform.
        /// \param background_to_use The TiledBackgroundBase argument of
        /// StreamingBackgroundBase
        /// \param map_tile_update_threshold The tilemap update threshold
        /// argument of StreamingBackgroundBase
        /// \param player_position The player position argument of
        /// StreamingBackgroundBase
        /// \param enable_wrapping The enable wrapping argument of
        /// StreamingBackgroundBase
        /// \return The StreamingBackgroundBase with sensible platform defaults
        static morpheus::core::gfx::StreamingBackgroundBase *construct_appropriate_streaming_background_base(
                morpheus::core::gfx::TiledBackgroundBase *background_to_use,
                morpheus::core::gfx::Vector2 map_tile_update_threshold = morpheus::core::gfx::Vector2(0, 0),
                const morpheus::core::gfx::Vector2 &player_position = morpheus::core::gfx::Vector2(0, 0),
                bool enable_wrapping = false) {
            morpheus::core::gfx::Vector2 screen_coordinates;

            #ifdef _GBA
                screen_coordinates = morpheus::core::gfx::Vector2(30, 20);
            #elif _NDS
                screen_coordinates = morpheus::core::gfx::Vector2(32, 24);
            #endif

            if(map_tile_update_threshold.get_x() <= 0) {
                map_tile_update_threshold = morpheus::core::gfx::Vector2(screen_coordinates.get_x(),
                                                                         map_tile_update_threshold.get_y());
            }

            if(map_tile_update_threshold.get_y() <= 0) {
                map_tile_update_threshold = morpheus::core::gfx::Vector2(map_tile_update_threshold.get_x(),
                                                                         screen_coordinates.get_y());
            }

            return new morpheus::core::gfx::StreamingBackgroundBase(background_to_use, map_tile_update_threshold,
                                                                    player_position, enable_wrapping);
        }
    }

    /// \file utils.hpp
    /// A file consisting mostly of helper functions constructing the
    /// appropriate implementations of Morpheus classes for code portability
    /// across the DS and the GBA.
}


#endif //MORPHEUS_GBA_TEST_UTILS_HPP
