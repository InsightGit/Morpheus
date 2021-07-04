//
// Created by bobby on 22/03/2021.
//

#ifndef MORPHEUS_GBA_TEST_SPRITE_BASE_HPP
#define MORPHEUS_GBA_TEST_SPRITE_BASE_HPP

#include <vector>

//#include <tonc.h>

#include <core/gfx/affine_interface.hpp>
#include <core/controllers.hpp>
#include <core/control_reciever.hpp>
#include <core/gfx/vector_2.hpp>

namespace morpheus {
    namespace core {
        namespace gfx {
            class AnimationFrame;
            class IntegerAnimationSmoothingAttribute;

            struct IntegerAnimationSmoothingAttributeDeleter {
                void operator()(IntegerAnimationSmoothingAttribute *ptr);
            };

            enum class SpriteSize {
                // square sprite sizes (x == y)
                SIZE_8X8, ///< Square 8px by 8px SpriteBase
                SIZE_16X16, ///< Square 16px by 16px SpriteBase
                SIZE_32X32, ///< Square 32px by 32px SpriteBase
                SIZE_64X64, ///< Square 64px by 64px SpriteBase

                // wide sprite sizes (x == 2 * y)
                SIZE_16X8, ///< Wide 16px by 8px SpriteBase
                SIZE_32X16, ///< Wide 32px by 16px SpriteBase
                SIZE_64X32, ///< Wide 64px by 32px SpriteBase

                // long sprite sizes (2 * x == y)
                SIZE_8X16, ///< Long 8px by 16px SpriteBase
                SIZE_16X32, ///< Long 16px by 32px SpriteBase
                SIZE_32X64 ///< Long 32px by 64px SpriteBase
            };


            /// \enum morpheus::core::gfx::SpriteSize
            /// An enum class of all the supported sizes of SpriteBases
            /// on the GBA and the DS. All sizes are given in terms of (X, Y).

            enum class AffineTransformation {
                Rotation, ///< Represents a Fixed-point rotation operation
                Scaling, ///< Represents a Fixed-point scaling operation
                Identity ///< Represents an Identity matrix reset operation
            };


            /// \enum morpheus::core::gfx::AffineTransformation
            /// An enum class of all the supported affine transformations
            /// supported on the GBA and the DS.

            class SpriteBase : public core::ControlReciever {
            public:
                /// Constructs a SpriteBase (also known as OBJ or objects
                /// within GBA/DS terminology) to be displayed.
                /// This SpriteBase can be either affine or regular, but make
                /// sure if it is affine that an affine video mode is active
                /// through core::MainLoop::enable_affine(). This class can
                /// also have blending and/or mosaic effects applied to it,
                /// but the blending_controller and mosaic_controller must be
                /// passed in for this SpriteBase to be affected by those
                /// graphical effects. Alternatively, passing in NULL or nullptr
                /// to those arguments will disable blending and/or mosaic on
                /// this SpriteBase.
                /// \param affine Whether this SpriteBase is affine or not
                /// \param blending_controller Which
                /// core::gfx::BlendingController to use (if any)
                /// \param mosaic_controller Which
                /// core::gfx::MosaicController to use (if any)
                SpriteBase(bool affine, BlendingController *blending_controller, MosaicController *mosaic_controller) {
                    m_affine = affine;
                    m_blending_controller = blending_controller;
                    m_mosaic_controller = mosaic_controller;
                }

                /// Destroys a SpriteBase.
                virtual ~SpriteBase() = default;

                /// \return The affine index currently being used for this
                /// affine sprite, or 32 if it is not an affine sprite.
                unsigned int get_affine_index() const {
                    if(m_affine) {
                        return m_affine_index;
                    } else {
                        return 32u;
                    }
                }

                /// \return The number of the current AnimationFrame this
                /// SpriteBase is currently on.
                unsigned int get_current_frame() const {
                    return m_current_frame;
                }

                /// \return The current mosaic levels of this SpriteBase,
                /// including horizontal and vertical mosaic.
                /// For more details on this effect, see
                /// the docs for morpheus::core::gfx::MosaicController
                core::gfx::Vector2 get_mosaic_levels() const {
                    if(m_mosaic_controller == nullptr) {
                        return core::gfx::Vector2();
                    } else {
                        return m_mosaic_controller->get_sprite_mosaic_levels();
                    }
                }

                /// \return The current global priority [0, 3] for this
                /// SpriteBase.
                unsigned int get_priority() const {
                    return m_priority;
                }

                /// \return The current screen position for this SpriteBase.
                core::gfx::Vector2 get_position() const {
                    return m_position;
                }

                /// \return The current fixed-point rotation for this
                /// affine SpriteBase (which is internally handled as an
                /// unsigned 16-bit short). If this SpriteBase is not affine,
                /// this function will just return 0.
                int get_rotation() const {
                    if(m_affine) {
                        return m_rotation;
                    } else {
                        return 0;
                    }
                }

                /// \return The current fixed-point scale for this
                /// affine SpriteBase. If this SpriteBase is not affine,
                /// this function will just return Vector2(0, 0).
                Vector2 get_scale() const {
                    if(m_affine) {
                        return m_scale;
                    } else {
                        return Vector2(0, 0);
                    }
                }

                /// \return Whether this SpriteBase is blended or not.
                virtual bool is_blended() const = 0;

                /// Disables blending on all SpriteBases (including this one).
                void disable_blending() {
                    if(m_blending_controller != nullptr) {
                        toggle_blending(false);
                    }
                }

                /// Enables blending on this specific SpriteBase and places it
                /// on either the bottom layer (also known as layer B or the
                /// 2nd layer) or the top layer (also known as layer A or the
                /// 1st layer).
                /// \param bottom_layer Whether to place this SpriteBase on
                /// the bottom layer (true) or the top layer (false)
                void enable_blending(bool bottom_layer) {
                    if(m_blending_controller != nullptr) {
                        toggle_blending(true, bottom_layer);
                    }
                }

                /// \return Whether this SpriteBase is effected by the mosaic
                /// effect.
                bool is_mosaic() const {
                    return m_mosaic;
                }

                /// \return The BlendingController being used in this SpriteBase
                /// that was passed in via the constructor.
                BlendingController *get_blending_controller() const {
                    return m_blending_controller;
                }

                /// \return The current AnimationFrames being used by
                /// this SpriteBase, if any.
                std::vector<std::shared_ptr<AnimationFrame>> get_frames() const {
                    return m_frames;
                }

                /// \return The MosaicController being used in this SpriteBase
                /// that was passed in via the constructor.
                MosaicController *get_mosaic_controller() const {
                    return m_mosaic_controller;
                }

                /// \return The frame to stop the SpriteBase animation at
                /// (or 0 if it stops at the end).
                unsigned int get_stop_frame() const {
                    return m_stop_frame;
                }

                /// Sets the index of the affine transformation matrix to use [0, 31]
                /// \param The index of the affine transformation matrix to use
                void set_affine_index(const unsigned int affine_index) {
                    if(m_affine) {
                        m_affine_index = std::min(31u, affine_index);
                    }
                }

                /// Sets the AnimationFrames this SpriteBase will use as its
                /// animation upon AnimationFrame::play() being called.
                /// Undefined behavior will occur if this function is called
                /// while another animation with a smooth transition is playing
                /// or is paused then resumed after this function call.
                /// \param frames The AnimationFrames this SpriteBase will use
                /// as its animation
                void set_frames(const std::vector<std::shared_ptr<AnimationFrame>> frames) {
                    m_frames.clear();

                    // deep vector copy
                    for(std::shared_ptr<AnimationFrame> frame : frames) {
                        m_frames.push_back(frame);
                    }
                }

                /// Sets the horizontal and vertical mosaic levels of
                /// this SpriteBase. For more details about this effect, see
                /// the documentation for morpheus::core::gfx::MosaicController
                /// \param mosaic_levels The Mosaic levels to be set for this
                /// SpriteBase.
                void set_mosaic_levels(const core::gfx::Vector2 mosaic_levels) {
                    if(m_mosaic_controller != nullptr) {
                        return m_mosaic_controller->set_sprite_mosaic_levels(mosaic_levels);
                    }
                }

                /// Sets the screen position of this SpriteBase.
                /// Bounds are the GBA screen size if running on the GBA
                /// (240x160 pixels) or the DS screen size if running on
                /// the DS (256x192 pixels on both screens).
                /// \param position The new screen position of this SpriteBase
                void set_position(const core::gfx::Vector2 position) {
                    m_position = position;
                }

                /// An overloader for the previous set_position() function
                /// taking a Vector2.
                void set_position(const int x, const int y) {
                    set_position(core::gfx::Vector2(x, y));
                }

                /// Sets the global priority [0, 3] for this SpriteBase. A
                /// Sprite with a lower global priority level than a
                /// background will be overlaid on the background and
                /// vice-versa.
                /// \param priority The new global priority for this SpriteBase
                void set_priority(unsigned int priority) {
                    m_priority = std::min(3u, priority);
                }

                /// Sets the frame to stop the SpriteBase animation at
                /// (or 0 if it stops at the end).
                /// This only applies when calling the stop() function
                /// or playing with animation looping disabled.
                void set_stop_frame(const unsigned int stop_frame) {
                    m_stop_frame = stop_frame;
                }

                /// Toggles whether mosaic should affect this SpriteBase.
                void toggle_mosaic() {
                    m_mosaic = !m_mosaic;

                    mosaic_state_updated();
                }

                /// Hides (but DOES NOT delete) this SpriteBase from being
                /// displayed.
                void hide() {
                    m_hidden = true;

                    on_visible_state_changed(m_hidden);
                }

                /// \return Whether this SpriteBase is affine or not.
                bool is_affine() const {
                    return m_affine;
                }

                /// \return Whether this SpriteBase is currently hidden from
                /// the display.
                bool is_hidden() const {
                    return m_hidden;
                }

                /// \return Whether this SpriteBase's animation is currently
                /// paused or not.
                bool is_paused() const {
                    return m_paused;
                }

                /// \return Whether this SpriteBase's animation is currently
                /// playing or not.
                bool is_playing() const {
                    return m_playing;
                }

                /// \return Whether this SpriteBase's animation is currently
                /// playing ON LOOP or not.
                bool is_playing_on_loop() const {
                    return m_looping && m_playing;
                }

                /// Shows or makes this SpriteBase visible on the display.
                void show() {
                    m_hidden = false;

                    on_visible_state_changed(m_hidden);
                }

                /// Sets the fixed-point rotation for this
                /// SpriteBase if this SpriteBase is affine.
                /// If this function is called on a non-affine SpriteBase,
                /// it will have no effect.
                /// \param rotation The new fixed-point rotation for this
                /// affine SpriteBase.
                void set_rotation(const short rotation) {
                    if(m_affine && m_rotation != rotation) {
                        m_rotation = rotation;

                        update_affine_state(AffineTransformation::Rotation,
                                            m_last_affine_transformation != AffineTransformation::Rotation);

                        m_last_affine_transformation = AffineTransformation::Rotation;
                    }
                }

                /// Sets the fixed-point scale Vector2 for this SpriteBase if
                /// this SpriteBase is affine. Vector2(1 << 8, 1 << 8) here
                /// represents the object at original size. If this function
                /// is called on a non-affine SpriteBase,
                /// it will have no effect.
                /// \param scale The new fixed-point scale for this affine
                /// SpriteBase.
                void set_scale(const core::gfx::Vector2 scale) {
                    if(m_affine && m_scale != scale) {
                        m_scale = core::gfx::Vector2(static_cast<short>(scale.get_x()),
                                                     static_cast<short>(scale.get_y()));

                        update_affine_state(AffineTransformation::Scaling,
                                            m_last_affine_transformation != AffineTransformation::Scaling);

                        m_last_affine_transformation = AffineTransformation::Scaling;
                    }
                }

                /// Draws this SpriteBase onto the screen by internally calling
                /// SpriteBase::draw_node(). Also takes a std::vector reference
                /// representing the object attribute buffer and number of max
                /// objects that can be placed in that buffer (however these
                /// arguments are only used on the GBA Sprite implementation
                /// currently).
                /// \param obj_attr_buffer Object attribute buffer for the GBA
                /// Sprite implementation
                /// \param obj_attr_num Vumber of max objects that can be
                /// placed in that buffer for the GBA Sprite implementation
                void draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num);

                /// \return The current SpriteSize of this SpriteBase
                virtual SpriteSize get_sprite_size() const = 0;

                /// Loads a grit-generated palette into this SpriteBase's
                /// color palette VRAM.
                /// \param palette The palette data
                /// \param pal_len The length (in bytes) of the palette data
                /// \param pal_offset The offset of the palette data load within
                /// the palette VRAM [0-255]
                /// \return Whether this was successfully loaded into the
                /// palette VRAM or not.
                virtual bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                               const unsigned int pal_offset = 256) = 0;

                /// Pauses the current animation on this SpriteBase at the
                /// current frame in the animation. If there is no current
                /// animation playing on this SpriteBase, this function has
                /// no effect.
                void pause() {
                    if(is_playing()) {
                        stop_animation(true);

                        m_playing = false;
                        m_paused = true;
                    }
                }

                /// Plays (or resumes if paused) the current animation on this
                /// SpriteBase as specified by SpriteBase::set_frames().
                /// If there are no AnimationFrames assigned to this
                /// SpriteBase through that aforementioned function, this
                /// function will have no effect.
                /// \param loop Whether to play this animation on loop or not
                void play(bool loop = true) {
                    if(!is_playing() && !m_frames.empty()) {
                        if(m_current_frame == 0) {
                            m_first_animation_cycle = true;
                        }

                        resume_animation();

                        m_looping = loop;
                        m_playing = true;
                        m_paused = false;
                    }
                }

                /// Stops the current animation on this SpriteBase and returns
                /// the modified SpriteBase attribute values to frame 0's values.
                void stop();

                /// Sets the SpriteSize of this SpriteBase.
                /// \param size The new SpriteSize to be set on this SpriteBase
                virtual void set_sprite_size(SpriteSize size) = 0;
            protected:
                /// Abstract method that is called by SpriteBase::draw() to
                /// actually draw the Sprite content. Also takes a std::vector reference
                /// representing the object attribute buffer and number of max
                /// objects that can be placed in that buffer (however these
                /// arguments are only used on the GBA Sprite implementation
                /// currently).
                /// \param obj_attr_buffer Object attribute buffer for the GBA
                /// Sprite implementation
                /// \param obj_attr_num Vumber of max objects that can be
                /// placed in that buffer for the GBA Sprite implementation
                virtual void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) = 0;

                /// Updates this SpriteBase to reflect the current sprite mosaic
                /// levels set in SpriteBase::set_mosaic_levels().
                virtual void mosaic_state_updated() = 0;

                /// Updates this SpriteBase's visible state to reflect
                /// whether this SpriteBase was recently hidden or shown
                /// through SpriteBase::hide() or SpriteBase::show().
                /// \param new_visible_state Whether this SpriteBase was newly
                /// shown (true) or hidden (false).
                virtual void on_visible_state_changed(bool new_visible_state) = 0;

                /// Updates the Blending enabled/disabled state of SpriteBases
                /// triggered through calling SpriteBase::disable_blending() and
                /// SpriteBase::enable_blending().
                /// \param enable_blending Whether this SpriteBase had blending
                /// newly enabled or disabled
                /// \param bottom_layer Whether this blending state change is
                /// on the bottom (true) or the top layer (false)
                virtual void toggle_blending(bool enable_blending, bool bottom_layer = true) = 0;

                /// Updates the affine state of this SpriteBase through
                /// applying an AffineTransformation.
                /// \param affine_transformation The AffineTransformation to
                /// apply
                /// \param new_transformation Whether this AffineTransformation
                /// was the last AffineTransformation applied to this SpriteBase
                virtual void update_affine_state(AffineTransformation affine_transformation,
                                                 bool new_transformation) = 0;

                /// Resumes or starts playing the current animation defined by
                /// SpriteBase::get_frames(). This should only be called by
                /// SpriteBase::play() when there is at least one AnimationFrame
                /// associated with this SpriteBase through
                /// SpriteBase::set_frames().
                virtual void resume_animation() = 0;

                /// Stops the current animation. This should only be called by
                /// SpriteBase::pause() or SpriteBase::stop() when an animation
                /// associated with this SpriteBase is playing.
                /// \param pause Whether to pause the current animation or
                /// completely stop it.
                virtual void stop_animation(bool pause) = 0;

                /// Updates the current animation. This should only be called
                /// in this sprite's implementation of SpriteBase::update()
                /// once per VBlank, or else animation will not be timed
                /// correctly.
                void update_animation();
            private:
                bool m_affine = false;
                unsigned int m_affine_index = 32;
                BlendingController *m_blending_controller;
                unsigned int m_current_delay;
                unsigned int m_current_frame = 0;
                bool m_first_animation_cycle = true;
                std::vector<std::shared_ptr<AnimationFrame>> m_frames;
                bool m_hidden = false;
                AffineTransformation m_last_affine_transformation = AffineTransformation::Identity;
                std::vector<std::unique_ptr<IntegerAnimationSmoothingAttribute,
                                            IntegerAnimationSmoothingAttributeDeleter>> m_linear_smoothing_attributes;
                bool m_looping = false;
                bool m_mosaic = false;
                MosaicController *m_mosaic_controller;
                Vector2 m_position;
                unsigned char m_priority = 0;
                bool m_paused = false;
                bool m_playing = false;
                int m_rotation = 0;
                Vector2 m_scale = Vector2(1 << 8, 1 << 8);
                bool m_smoothing_started = false;
                int m_smoothing_trend;
                int m_stop_frame = 0;
            };


            /// \class morpheus::core::gfx::SpriteBase
            ///
            /// The abstract base class that all sprite objects should inherit.
            /// Sprite objects, otherwise known as objects or OBJ,
            /// can be controlled by the player, modified by graphical effects
            /// like mosaic or blending, animated through AnimationFrames, and
            /// can be rotated and scaled (if affine).
            /// For an example of Sprites being used, see Input Test (tests/input_test/input_test.cpp),
            /// Graphical Effect Test (tests/gfx_effects_test/gfx_effect_test.cpp),
            /// Animation Test (tests/animation_test/animation_test.cpp) and
            /// Affine Test (tests/affine_test/affine_test.cpp),
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_BASE_HPP
