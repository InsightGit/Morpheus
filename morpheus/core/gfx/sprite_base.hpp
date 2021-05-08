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
                SIZE_8X8,
                SIZE_16X16,
                SIZE_32X32,
                SIZE_64X64,

                // wide sprite sizes (x == 2 * y)
                SIZE_16X8,
                SIZE_32X16,
                SIZE_64X32,

                // long sprite sizes (2 * x == y)
                SIZE_8X16,
                SIZE_16X32,
                SIZE_32X64
            };

            enum class AffineTransformation {
                Rotation,
                Scaling,
                Identity
            };

            class SpriteBase : public core::ControlReciever {
            public:
                SpriteBase(bool affine, BlendingController *blending_controller, MosaicController *mosaic_controller) {
                    m_affine = affine;
                    m_blending_controller = blending_controller;
                    m_mosaic_controller = mosaic_controller;
                }

                virtual ~SpriteBase() {}

                unsigned int get_affine_index() const {
                    if(m_affine) {
                        return m_affine_index;
                    } else {
                        return 32u;
                    }
                }

                unsigned int get_current_frame() const {
                    return m_current_frame;
                }

                core::gfx::Vector2 get_mosaic_levels() const {
                    if(m_mosaic_controller == nullptr) {
                        return core::gfx::Vector2();
                    } else {
                        return m_mosaic_controller->get_sprite_mosaic_levels();
                    }
                }

                unsigned int get_priority() const {
                    return m_priority;
                }

                core::gfx::Vector2 get_position() const {
                    return m_position;
                }

                int get_rotation() const {
                    if(m_affine) {
                        return m_rotation;
                    } else {
                        return 0;
                    }
                }

                Vector2 get_scale() const {
                    if(m_affine) {
                        return m_scale;
                    } else {
                        return Vector2(1, 1);
                    }
                }

                virtual bool is_blended() const = 0;

                void disable_blending() {
                    if(m_blending_controller != nullptr) {
                        toggle_blending(false);
                    }
                }

                void enable_blending(bool bottom_layer) {
                    if(m_blending_controller != nullptr) {
                        toggle_blending(true, bottom_layer);
                    }
                }

                bool is_mosaic() const {
                    return m_mosaic;
                }

                BlendingController *get_blending_controller() const {
                    return m_blending_controller;
                }

                std::vector<std::shared_ptr<AnimationFrame>> get_frames() const {
                    return m_frames;
                }

                MosaicController *get_mosaic_controller() const {
                    return m_mosaic_controller;
                }

                void set_affine_index(const unsigned int affine_index) {
                    if(m_affine) {
                        m_affine_index = std::min(31u, affine_index);
                    }
                }

                void set_frames(const std::vector<std::shared_ptr<AnimationFrame>> frames) {
                    m_frames.clear();

                    // deep vector copy
                    for(std::shared_ptr<AnimationFrame> frame : frames) {
                        m_frames.push_back(frame);
                    }
                }

                void set_mosaic_levels(const core::gfx::Vector2 mosaic_levels) {
                    if(m_mosaic_controller != nullptr) {
                        return m_mosaic_controller->set_sprite_mosaic_levels(mosaic_levels);
                    }
                }

                void set_position(const core::gfx::Vector2 position) {
                    m_position = position;
                }

                void set_position(const int x, const int y) {
                    set_position(core::gfx::Vector2(x, y));
                }

                void set_priority(unsigned int priority) {
                    m_priority = std::min(3u, priority);
                }

                void toggle_mosaic() {
                    m_mosaic = !m_mosaic;

                    mosaic_state_updated();
                }

                void hide() {
                    m_hidden = true;

                    on_visible_state_changed(m_hidden);
                }

                bool is_affine() const {
                    return m_affine;
                }

                bool is_drawn_node() const {
                    return m_drawn_node;
                }

                bool is_hidden() const {
                    return m_hidden;
                }

                bool is_paused() const {
                    return m_paused;
                }

                bool is_playing() const {
                    return m_playing;
                }

                void set_drawn_node(const bool drawn_node) {
                    m_drawn_node = drawn_node;
                }

                void show() {
                    m_hidden = false;

                    on_visible_state_changed(m_hidden);
                }

                void set_rotation(const short rotation) {
                    if(m_affine && m_rotation != rotation) {
                        m_rotation = rotation;

                        update_affine_state(AffineTransformation::Rotation,
                                            m_last_affine_transformation != AffineTransformation::Rotation);

                        m_last_affine_transformation = AffineTransformation::Rotation;
                    }
                }

                void set_scale(const core::gfx::Vector2 scale) {
                    if(m_affine && m_scale != scale) {
                        m_scale = core::gfx::Vector2(static_cast<short>(scale.get_x()),
                                                     static_cast<short>(scale.get_y()));

                        update_affine_state(AffineTransformation::Scaling,
                                            m_last_affine_transformation != AffineTransformation::Scaling);

                        m_last_affine_transformation = AffineTransformation::Scaling;
                    }
                }

                void draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num);

                virtual SpriteSize get_sprite_size() const = 0;
                virtual bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                                               const unsigned int pal_offset = 256) = 0;

                void pause() {
                    if(is_playing()) {
                        stop_animation(true);

                        m_playing = false;
                        m_paused = true;
                    }
                }

                void play() {
                    if(!is_playing()) {
                        if(m_current_frame == 0) {
                            m_first_animation_cycle = true;
                        }

                        resume_animation();

                        m_playing = true;
                        m_paused = false;
                    }
                }

                void stop() {
                    stop_animation(false);

                    m_playing = false;
                    m_paused = false;
                }

                virtual void set_sprite_size(SpriteSize size) = 0;
            protected:
                virtual void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) = 0;
                virtual void mosaic_state_updated() = 0;
                virtual void on_visible_state_changed(bool new_visible_state) = 0;
                virtual void toggle_blending(bool enable_blending, bool bottom_layer = true) = 0;
                virtual void update_affine_state(AffineTransformation affine_transformation,
                                                 bool new_transformation) = 0;

                virtual void resume_animation() = 0;
                virtual void stop_animation(bool pause) = 0;

                // should only be called once per VBlank
                void update_animation();
            private:
                bool m_affine = false;
                unsigned int m_affine_index = 32;
                BlendingController *m_blending_controller;
                unsigned int m_current_delay;
                unsigned int m_current_frame = 0;
                bool m_drawn_node = true;
                bool m_first_animation_cycle = true;
                std::vector<std::shared_ptr<AnimationFrame>> m_frames;
                bool m_hidden = false;
                AffineTransformation m_last_affine_transformation = AffineTransformation::Identity;
                std::vector<std::unique_ptr<IntegerAnimationSmoothingAttribute,
                                            IntegerAnimationSmoothingAttributeDeleter>> m_linear_smoothing_attributes;
                bool m_mosaic = false;
                MosaicController *m_mosaic_controller;
                Vector2 m_position;
                unsigned char m_priority = 0;
                bool m_paused = false;
                bool m_playing = false;
                Vector2 m_scale = Vector2(1 << 8, 1 << 8);
                bool m_smoothing_started = false;
                int m_smoothing_trend;
                int m_rotation = 0;
            };
        }
    }
}

#endif //MORPHEUS_GBA_TEST_SPRITE_BASE_HPP
