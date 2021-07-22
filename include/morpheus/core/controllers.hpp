//
// Created by bobby on 20/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_CONTROLLERS_HPP
#define MORPHEUS_GBA_TEST_CONTROLLERS_HPP

#include <array>

#include "uncopyable.hpp"
#include "gfx/vector_2.hpp"

namespace morpheus {
    namespace core {
        namespace gfx {
            enum class BlendingMode {
                OFF, ///< Blending off
                USE_WEIGHTS, ///< Alpha-blending using weights (making
                             ///< certain backgrounds or objects look
                             ///< "transparent")
                FADE_TO_WHITE, ///< Fades certain backgrounds or objects
                               ///< to white
                FADE_TO_BLACK ///< Fades certain backgrounds or objects
                              ///< to black
            };

            /// \enum morpheus::core::gfx::BlendingMode
            /// Enum class for the current BlendingMode, which determines the
            /// type of blending active. This can include fading the top layers
            /// to black (BlendingMode::FADE_TO_BLACK) or white
            /// (BlendingMode::FADE_TO_WHITE), or using the weights from
            /// the bottom and the top layer to blend each other together.
            /// (BlendingMode::USE_WEIGHTS).

            enum class BlendingSetting {
                OFF, ///< This background/object is not affected by the blending
                     ///< effect
                BOTTOM_ON, ///< This background/object is on the bottom layer of
                           ///< the blending effect
                TOP_ON, ///< This background/object is on the top layer of
                        ///< the blending effect
                INVALID ///< Invalid BlendingSetting returned during function
                        ///< errors
            };

            /// \enum morpheus::core::gfx::BlendingSetting
            /// Internally used enum class for the current BlendingSetting,
            /// which determines if
            /// blending is on for a certain object/background or whether it
            /// is on thebottom layer or the top layer. NOTE that bottom layers
            /// must actually be behind top layer while using
            /// BlendingMode::USE_WEIGHTS for the blending to work and that
            /// BlendingMode::FADE_TO_BLACK and BlendingMode::FADE_TO_WHITE
            /// only work on layers on the top.

            class BlendingController : Uncopyable {
            public:
                /// Constructs a BlendingController. Because this is a
                /// controller class, this should only be done once and within
                /// the initialization of the morpheus::core::MainLoop.
                BlendingController() = default;

                /// Destructs the current BlendingController.
                virtual ~BlendingController() {}

                /// Disables blending for the backdrop (or a blank background
                /// consisting of only the color 0).
                void disable_backdrop_blending() {
                    m_backdrop_blending_setting = BlendingSetting::OFF;

                    update_blending_registers();
                }

                /// Enables blending for the backdrop (or a blank background
                /// consisting of only the color 0).
                ///
                /// \param bottom Whether to place the backdrop on the bottom
                /// blending layer (true) or the top blending layer (false).
                void enable_backdrop_blending(bool bottom) {
                    if(bottom) {
                        m_backdrop_blending_setting = BlendingSetting::BOTTOM_ON;
                    } else {
                        m_backdrop_blending_setting = BlendingSetting::TOP_ON;
                    }

                    update_blending_registers();
                }

                /// \return The current backdrop BlendingSetting
                BlendingSetting get_backdrop_blending_setting() const {
                    return m_backdrop_blending_setting;
                }

                /// Disables blending for a certain background.
                ///
                /// \param background The background number to disable blending on.
                void disable_background_blending(unsigned int background) {
                    if(background < 4) {
                        m_background_blending_settings[background] = BlendingSetting::OFF;
                    }

                    update_blending_registers();
                }

                /// Enables blending for a certain background.
                /// \param bottom Whether to place the backdrop on the bottom
                /// blending layer (true) or the top blending layer (false).
                /// \param background The background number to enable blending on.
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

                /// \param background The background number to get the
                /// BlendingSetting from.
                /// \return The BlendingSetting of the background given.
                /// If no such background exists with the background number
                /// given, BlendingSetting::INVALID will be returned.
                BlendingSetting get_background_blending_setting(unsigned int background) const {
                    if(m_background_blending_settings.size() > background) {
                        return m_background_blending_settings[background];
                    } else {
                        return BlendingSetting::INVALID;
                    }
                }

                /// Disables blending for all objects(SpriteBases), regardless
                /// of whether the Sprite has individually activated blending
                /// or not through SpriteBase::enable_blending().
                void disable_object_blending() {
                    m_object_blending_setting = BlendingSetting::OFF;

                    update_blending_registers();
                }

                /// Enables blending for all objects that have enabled
                /// blending. To enable blending for a SpriteBase, call
                /// SpriteBase::enable_blending().
                ///
                /// \param bottom Whether to place objects on the bottom
                /// blending layer (true) or the top blending layer (false).
                void enable_object_blending(bool bottom) {
                    if(bottom) {
                        m_object_blending_setting = BlendingSetting::BOTTOM_ON;
                    } else {
                        m_object_blending_setting = BlendingSetting::TOP_ON;
                    }

                    update_blending_registers();
                }

                /// \return The current BlendingSetting of the objects.
                BlendingSetting get_object_blending_setting() const {
                    return m_object_blending_setting;
                }

                /// \return The current BlendingMode of this BlendingController.
                BlendingMode get_blending_mode() const {
                    return m_current_blending_mode;
                }

                /// Sets the BlendingMode of this BlendingController.
                /// For more information on each BlendingMode,
                /// see the enum class's documentation.
                /// \param blending_mode The new BlendingMode for
                /// this BlendingController.
                void set_blending_mode(BlendingMode blending_mode) {
                    m_current_blending_mode = blending_mode;

                    update_blending_registers();
                }

                /// \param bottom Whether the blending weight of the bottom or
                /// the top layer will be returned.
                /// \return The current blending weight of either the bottom or
                /// the top layer. Blending weights are restricted to numbers
                /// 0-16, and are only used when the current BlendingMode of
                /// this BlendingController is BlendingMode::USE_WEIGHTS.
                unsigned char get_blend_weight(bool bottom) const {
                    if(bottom) {
                        return m_bottom_blend_weight;
                    } else {
                        return m_top_blend_weight;
                    }
                }

                /// Sets the blending weight of the bottom and the top layers.
                /// While only one weight (that must be between 0 and 16)
                /// is passed in, the other weight is calculated to be
                /// 16 - the weight passed in. These weights are only used when
                /// the current BlendingMode of this BlendingController is
                /// BlendingMode::USE_WEIGHTS.
                /// \param bottom Whether to set the bottom or the top layer to
                /// the weight passed in.
                /// \param weight The weight to set the bottom or the top layer.
                void set_blend_weight(bool bottom, unsigned char weight) {
                    if(bottom) {
                        m_bottom_blend_weight = std::min(weight, static_cast<unsigned char>(16));
                        m_top_blend_weight = 16 - m_bottom_blend_weight;
                    } else {
                        m_top_blend_weight = std::min(weight, static_cast<unsigned char>(16));

                        m_bottom_blend_weight = 16 - m_top_blend_weight;
                    }

                    update_blending_registers();
                }

                /// \return The current blending fade (that must be between
                /// 0 and 16). This value will only be used when the current
                /// BlendingMode of this BlendingController is
                /// BlendingMode::FADE_TO_BLACK or BlendingMode::FADE_TO_WHITE.
                unsigned char get_blend_fade() const {
                    return m_blend_fade;
                }

                /// Sets the current blending fade or the intensity of the fade
                /// effect (that must be between 0 and 16). This value will
                /// only be used when the current BlendingMode of this
                /// BlendingController is BlendingMode::FADE_TO_BLACK or
                /// BlendingMode::FADE_TO_WHITE.
                /// \param fade The current blending fade to be set
                void set_blend_fade(unsigned char fade) {
                    m_blend_fade = std::min(fade, static_cast<unsigned char>(16));

                    update_blending_registers();
                }
            protected:
                std::array<BlendingSetting, 4> get_background_blending_settings() const {
                    return m_background_blending_settings;
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


            /// \class morpheus::core::gfx::BlendingController
            /// A controller class controlling the state of the
            /// blending registers on the GBA and the DS. This should only
            /// be constructed once per screen within the MainLoop.
            /// Afterwards, this object can be accessed by calling
            /// morpheus::core::MainLoop::get_blending_controller() or
            /// morpheus::nds::NdsMainLoop::get_sub_blending_controller()
            /// on the DS for its blending controller for the sub-screen.
            /// It supports all three blending modes: Fade to black or white,
            /// or weight blending between a bottom and a top layer.

            class MosaicController : Uncopyable {
            public:
                /// Destructs the current MosaicController.
                virtual ~MosaicController() {}

                /// \return The current mosaic levels (0-15 for both x and y)
                /// for enabled backgrounds.
                Vector2 get_background_mosaic_levels() const {
                    return m_background_mosaic_levels;
                }

                /// \return The current mosaic levels (0-15 for both x and y)
                /// for enabled sprites.
                Vector2 get_sprite_mosaic_levels() const {
                    return m_sprite_mosaic_levels;
                }

                /// Sets mosaic levels (which must be between 0-15) of all
                /// enabled backgrounds to background_mosaic_levels.
                /// \param background_mosaic_levels The mosaic levels to set
                /// enabled backgrounds to
                void set_background_mosaic_levels(const Vector2 background_mosaic_levels) {
                    m_background_mosaic_levels = core::gfx::Vector2(
                                                        std::max(std::min(15, background_mosaic_levels.get_x()), 0),
                                                        std::max(std::min(15, background_mosaic_levels.get_y()), 0));

                    update_mosaic_register();
                }

                /// Set mosaic levels (which must be between 0-15) of all
                /// enabled sprites to sprite_mosaic_levels.
                /// \param sprite_mosaic_levels The mosaic levels to set enabled sprites to
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


            /// \class morpheus::core::gfx::MosaicController
            /// A controller class controlling the state of the
            /// mosaic registers on the GBA and the DS. This should only
            /// be constructed once per screen within the MainLoop.
            /// Afterwards, this object can be accessed by calling
            /// morpheus::core::MainLoop::get_mosaic_controller() or
            /// morpheus::nds::NdsMainLoop::get_sub_mosaic_controller()
            /// on the DS for its mosaic controller for the sub-screen.
            /// The mosaic effect on these systems make the pixels look
            /// more blocky and distorted. For a demo of the effect in action,
            /// see ../tests/gfx_effects_test/gfx_effects_test.cpp
        }

        class NoCashDebugController : Uncopyable {
        public:
            /// Destructs this NoCashDebugController.
            virtual ~NoCashDebugController() = default;

            /// Sends a custom debug message to NO$GBA's debug window,
            /// which can be found in the debugger/developer version
            /// (not the gaming version) of NO$GBA, under the "Window" menu and
            /// then the "TTY Debug Messages" tab. In order to see the
            /// messages sent by this function, insure that Debug I/O is
            /// enabled under the "Options" menu and then the
            /// "Debugger Setup" tab.
            /// \param message The message to send to the NO$GBA debug window
            virtual void send_to_debug_window(std::string message) = 0;
        };


        /// \class morpheus::core::NoCashDebugController
        /// A class that sends debug messages as specified by the game
        /// to NO$GBA's debug window. See
        /// NoCashDebugController::send_to_debug_window() for more details.
    }
}

#endif //MORPHEUS_GBA_TEST_CONTROLLERS_HPP
