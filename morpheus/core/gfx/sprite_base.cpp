//
// Created by bobby on 22/03/2021.
//

#include "sprite_base.hpp"

#include <core/control_reciever.hpp>
#include <core/gfx/animation_frame.hpp>
#include <core/gfx/animation_smoothing_attributes.hpp>

void morpheus::core::gfx::SpriteBase::draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    if(obj_attr_num >= 128) {
        std::cout << "ERROR: obj_attr_num exceeded max of 127!";
    } else if(is_drawn_node()) {
        draw_node(obj_attr_buffer, obj_attr_num);
        if(std::is_base_of<core::gfx::AffineInterface, typeof(this)>::value) {
            reinterpret_cast<core::gfx::AffineInterface*>(this)->draw_affine_node(obj_attr_buffer, obj_attr_num);
        }
    }
}

void morpheus::core::gfx::SpriteBase::update_animation() {
    if(is_playing()) {
        ++m_current_delay;

        if(!m_frames.empty() && m_current_delay >= m_frames[m_current_frame]->get_vblank_delays()) {
            if(m_current_frame + 1 >= m_frames.size()) {
                m_current_frame = 0;
                m_first_animation_cycle = false;

                //nocash_puts("playing frame 0");
            } else {
                ++m_current_frame;

                //nocash_puts(std::string("playing frame " + std::to_string(m_current_frame)).c_str());
            }

            m_current_delay = 0;
            m_smoothing_started = false;

            //nocash_puts(std::string("Activating current frame " + std::to_string(get_current_frame())).c_str());

            m_frames[get_current_frame()]->activate_on_target_sprite_base();

            return;
        }

        if(m_smoothing_started) {
            for(std::unique_ptr<IntegerAnimationSmoothingAttribute,
                    IntegerAnimationSmoothingAttributeDeleter> &smoothing_attribute : m_linear_smoothing_attributes) {
                smoothing_attribute->smooth();

                //nocash_puts("smoothing");
            }
        } else if(m_frames.size() > 1) {
            std::shared_ptr<AnimationFrame> current_frame = m_frames[m_current_frame];
            std::shared_ptr<AnimationFrame> next_frame;
            unsigned int num_of_vblanks;

            if(m_current_frame + 1 >= m_frames.size()) {
                next_frame = m_frames[0];

                //nocash_puts("next frame is frame 0");
            } else {
                next_frame = m_frames[m_current_frame + 1];

                //nocash_puts(std::string("next frame is frame " + std::to_string(m_current_frame + 1)).c_str());
            }

            num_of_vblanks = current_frame->get_vblank_delays();

            m_linear_smoothing_attributes.clear();

            for(AnimationFrameCopyOption copy_option : current_frame->
                get_copy_options_with_smoothing_mode(core::gfx::AnimationSmoothingMode::LINEAR)) {
                int from_value = 0;
                int to_value = 0;
                core::gfx::Vector2 from_vector_value;
                core::gfx::Vector2 to_vector_value;
                bool invalid_copy_option = false;
                bool using_vector = false;

                switch (copy_option) {
                    case AnimationFrameCopyOption::BLENDING:
                        from_value = current_frame->get_blending_value();
                        to_value = next_frame->get_blending_value();
                        break;
                    case AnimationFrameCopyOption::SPRITE_SIZE:
                        break;
                    case AnimationFrameCopyOption::MOSAIC:
                        from_vector_value = current_frame->get_mosaic_levels();
                        to_vector_value = next_frame->get_mosaic_levels();
                        using_vector = true;
                        break;
                    case AnimationFrameCopyOption::PALETTE:
                        break;
                    case AnimationFrameCopyOption::POSITION:
                        from_vector_value = current_frame->get_position();
                        to_vector_value = next_frame->get_position();
                        using_vector = true;
                        break;
                    case AnimationFrameCopyOption::ROTATION:
                        from_value = current_frame->get_rotation();
                        to_value = next_frame->get_rotation();
                        break;
                    case AnimationFrameCopyOption::SCALE:
                        from_vector_value = current_frame->get_scale();
                        to_vector_value = next_frame->get_scale();
                        using_vector = true;
                        break;
                    default:
                        invalid_copy_option = true;
                        break;
                }

                if(invalid_copy_option) {
                    continue;
                }

                if(using_vector) {
                    m_linear_smoothing_attributes.emplace_back(
                            new core::gfx::Vector2SmoothingAttribute(this, copy_option, from_vector_value,
                                                                     to_vector_value, num_of_vblanks));
                } else {
                    m_linear_smoothing_attributes.emplace_back(
                            new core::gfx::IntegerAnimationSmoothingAttribute(this, copy_option, from_value, to_value,
                                                                              num_of_vblanks));
                }
            }

            m_smoothing_started = true;
        }
    }
}

void morpheus::core::gfx::IntegerAnimationSmoothingAttributeDeleter::operator()(
        morpheus::core::gfx::IntegerAnimationSmoothingAttribute *ptr) {
    delete ptr;
}
