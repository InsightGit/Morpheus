//
// Created by bobby on 28/04/2021.
//

#include "morpheus/core/gfx/animation_smoothing_attributes.hpp"

morpheus::core::gfx::IntegerAnimationSmoothingAttribute::IntegerAnimationSmoothingAttribute(
        morpheus::core::gfx::SpriteBase *target_sprite, morpheus::core::gfx::AnimationFrameCopyOption copy_option,
        int from, int to, unsigned int in_vblanks) {
    int signed_remainder;
    unsigned int remainder;

    m_copy_option = copy_option;
    m_goal = to;
    m_target_sprite = target_sprite;

    if(to - from != 0) {
        m_trend = (to - from) / static_cast<int>(in_vblanks);

        signed_remainder = (to - from) % static_cast<int>(in_vblanks);
        remainder = abs(signed_remainder);

        m_decreasing = signed_remainder < 0;

        if(remainder != 0) {
            if(remainder > (in_vblanks / 2)) {
                m_compensation = 1;
            } else {
                m_compensation = in_vblanks / remainder;
            }
        }
    }
}

morpheus::core::gfx::Vector2SmoothingAttribute::Vector2SmoothingAttribute(
        morpheus::core::gfx::SpriteBase *target_sprite, morpheus::core::gfx::AnimationFrameCopyOption copy_option,
        morpheus::core::gfx::Vector2 from, morpheus::core::gfx::Vector2 to, unsigned int in_vblanks) :
        IntegerAnimationSmoothingAttribute(target_sprite, copy_option, 0, 0, 0) {

    int signed_remainder;
    unsigned int remainder;

    m_vector_compensation = core::gfx::Vector2(static_cast<int>(in_vblanks), static_cast<int>(in_vblanks));
    m_vector_frame_counter = core::gfx::Vector2(0, 0);
    m_vector_goal = to;
    m_vector_trend = core::gfx::Vector2(0, 0);

    //nocash_puts(std::string("from vector: " + from.to_string()).c_str());
    //nocash_puts(std::string("to vector: " + to.to_string()).c_str());

    if(to.get_x() - from.get_x() != 0) {
        m_vector_trend = core::gfx::Vector2((to.get_x() - from.get_x()) / static_cast<int>(in_vblanks),
                                            m_vector_trend.get_y());

        signed_remainder = (to.get_x() - from.get_x()) % static_cast<int>(in_vblanks);
        remainder = abs(signed_remainder);

        m_decreasing_x = signed_remainder < 0;

        if(remainder != 0) {
            //nocash_puts(std::string("x remainder: " + std::to_string(remainder)).c_str());

            if(m_decreasing_x) {
                //nocash_puts("decreasing");
            } else {
                //nocash_puts("increasing");
            }

            if(remainder > (in_vblanks / 2)) {
                // TODO(Bobby): Figure out something better than this to do:
                m_vector_compensation = core::gfx::Vector2(1, 1);
            } else {
                m_vector_compensation = core::gfx::Vector2(in_vblanks / remainder, m_vector_compensation.get_y());
            }
        }
    }

    if(to.get_y() - from.get_y() != 0) {
        m_vector_trend = core::gfx::Vector2(m_vector_trend.get_x(),
                                            (to.get_y() - from.get_y()) / static_cast<int>(in_vblanks));

        signed_remainder = (to.get_y() - from.get_y()) % static_cast<int>(in_vblanks);
        remainder = abs(signed_remainder);

        m_decreasing_y = signed_remainder < 0;

        if(remainder != 0) {
            //nocash_puts(std::string("y remainder: " + std::to_string(remainder)).c_str());

            if(m_decreasing_y) {
                //nocash_puts("decreasing");
            } else {
                //nocash_puts("increasing");
            }

            if(remainder > (in_vblanks / 2)) {
                // TODO(Bobby): Figure out something better than this to do:
                m_vector_compensation = core::gfx::Vector2(1, 1);
            } else {
                m_vector_compensation = core::gfx::Vector2(m_vector_compensation.get_x(), in_vblanks / remainder);
            }
        }
    }

    if(m_vector_compensation != core::gfx::Vector2(static_cast<int>(in_vblanks), static_cast<int>(in_vblanks))) {
        //nocash_puts(std::string("Vector compensation: " + m_vector_compensation.to_string()).c_str());
    }

    //nocash_puts(std::string("Vector trend: " + m_vector_trend.to_string()).c_str());
    //nocash_puts(std::string("in " + std::to_string(in_vblanks) + " vblanks").c_str());
}

bool morpheus::core::gfx::IntegerAnimationSmoothingAttribute::smooth() {
    int trend_addition_value = m_trend;

    ++m_frame_counter;

    if(m_frame_counter >= m_compensation) {
        m_frame_counter = 0;

        if(m_decreasing) {
            --trend_addition_value;
        } else {
            ++trend_addition_value;
        }
    }

    switch (m_copy_option) {
        case AnimationFrameCopyOption::BLENDING: {
            BlendingController *blending_controller = m_target_sprite->get_blending_controller();

            switch (blending_controller->get_blending_mode()) {
                case BlendingMode::USE_WEIGHTS:
                    switch (blending_controller->get_object_blending_setting()) {
                        case BlendingSetting::BOTTOM_ON:
                            blending_controller->set_blend_weight(
                                    true,
                                    get_min_value(blending_controller->get_blend_weight(true) + trend_addition_value));
                            break;
                        case BlendingSetting::TOP_ON:
                            blending_controller->set_blend_weight(
                                    false,
                                    get_min_value(blending_controller->get_blend_weight(false) + trend_addition_value));
                            break;
                        case BlendingSetting::INVALID:
                        case BlendingSetting::OFF:
                            return false;
                    }
                    break;
                case BlendingMode::FADE_TO_WHITE:
                case BlendingMode::FADE_TO_BLACK: {
                    int blend_fade = get_min_value(blending_controller->get_blend_fade() + trend_addition_value);

                    blending_controller->set_blend_fade(blend_fade);

                    break;
                }
                case BlendingMode::OFF:
                    return false;
            }
            break;
        }
        case AnimationFrameCopyOption::SPRITE_SIZE:
        case AnimationFrameCopyOption::PALETTE:
            // TODO(Bobby): Determine whether we can should even support smoothing of these attributes
            return false;
        case AnimationFrameCopyOption::ROTATION:
            if (m_target_sprite->is_affine()) {
                m_target_sprite->set_rotation(get_min_value(m_target_sprite->get_rotation() + m_trend));
            } else {
                return false;
            }
        default:
            return false;
    }

    return true;
}

bool morpheus::core::gfx::Vector2SmoothingAttribute::smooth() {
    core::gfx::Vector2 trend_addition_vector = m_vector_trend;

    m_vector_frame_counter = core::gfx::Vector2(m_vector_frame_counter.get_x() + 1,
                                                m_vector_frame_counter.get_y() + 1);

    if(m_vector_frame_counter.get_x() >= m_vector_compensation.get_x()) {
        m_vector_frame_counter = core::gfx::Vector2(0, m_vector_frame_counter.get_y());

        if(m_decreasing_x) {
            trend_addition_vector = core::gfx::Vector2(trend_addition_vector.get_x() - 1,
                                                       trend_addition_vector.get_y());
        } else {
            trend_addition_vector = core::gfx::Vector2(trend_addition_vector.get_x() + 1,
                                                       trend_addition_vector.get_y());
        }
    }

    if(m_vector_frame_counter.get_y() >= m_vector_compensation.get_y()) {
        m_vector_frame_counter = core::gfx::Vector2(m_vector_frame_counter.get_x(), 0);

        if(m_decreasing_y) {
            trend_addition_vector = core::gfx::Vector2(trend_addition_vector.get_x(),
                                                       trend_addition_vector.get_y() - 1);
        } else {
            trend_addition_vector = core::gfx::Vector2(trend_addition_vector.get_x(),
                                                       trend_addition_vector.get_y() + 1);
        }
    }

    switch (get_copy_option()) {
        case AnimationFrameCopyOption::MOSAIC:
            get_target_sprite()->set_mosaic_levels(
                    get_min_vector(get_target_sprite()->get_mosaic_levels() + trend_addition_vector));
            break;
        case AnimationFrameCopyOption::POSITION:
            get_target_sprite()->set_position(get_min_vector(get_target_sprite()->get_position() +
                                                                            trend_addition_vector));
            break;
        case AnimationFrameCopyOption::SCALE:
            if(get_target_sprite()->is_affine()) {
                get_target_sprite()->set_scale(get_min_vector(get_target_sprite()->get_scale() +
                                                              trend_addition_vector));
            } else {
                return false;
            }
            break;
        default:
            return false;
    }

    return true;
}

morpheus::core::gfx::Vector2 morpheus::core::gfx::Vector2SmoothingAttribute::get_min_vector(core::gfx::Vector2 vector) {
    if(m_decreasing_y && m_decreasing_x) {
        return core::gfx::Vector2(std::max(m_vector_goal.get_x(), vector.get_x()),
                                  std::max(m_vector_goal.get_y(), vector.get_y()));
    } else if(m_decreasing_y) {
        return core::gfx::Vector2(std::min(m_vector_goal.get_x(), vector.get_x()),
                                  std::max(m_vector_goal.get_y(), vector.get_y()));
    } else if(m_decreasing_x) {
        return core::gfx::Vector2(std::max(m_vector_goal.get_x(), vector.get_x()),
                                  std::min(m_vector_goal.get_y(), vector.get_y()));
    } else {
        return core::gfx::Vector2(std::min(m_vector_goal.get_x(), vector.get_x()),
                                  std::min(m_vector_goal.get_y(), vector.get_y()));
    }
}
