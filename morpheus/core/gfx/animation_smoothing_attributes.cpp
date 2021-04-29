//
// Created by bobby on 28/04/2021.
//

#include "animation_smoothing_attributes.hpp"

bool morpheus::core::gfx::IntegerAnimationSmoothingAttribute::smooth() {
    ++m_frame_count;

    if (m_frame_count >= m_trend_per_frames) {
        switch (m_copy_option) {
            case AnimationFrameCopyOption::BLENDING: {
                BlendingController *blending_controller = m_target_sprite->get_blending_controller();

                switch (blending_controller->get_blending_mode()) {
                    case BlendingMode::USE_WEIGHTS:
                        switch (blending_controller->get_object_blending_setting()) {
                            case BlendingSetting::BOTTOM_ON:
                                blending_controller->set_blend_weight(
                                        true,
                                        blending_controller->get_blend_weight(true) + m_trend);
                                break;
                            case BlendingSetting::TOP_ON:
                                blending_controller->set_blend_weight(
                                        false,
                                        blending_controller->get_blend_weight(false) + m_trend);
                                break;
                            case BlendingSetting::OFF:
                                return false;
                        }
                        break;
                    case BlendingMode::FADE_TO_WHITE:
                    case BlendingMode::FADE_TO_BLACK:
                        blending_controller->set_blend_fade(blending_controller->get_blend_fade() + m_trend);
                        break;
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
                    m_target_sprite->set_rotation(m_target_sprite->get_rotation() + m_trend);
                } else {
                    return false;
                }
            default:
                return false;
        }

        m_frame_count = 0;
    }

    return true;
}

bool morpheus::core::gfx::Vector2SmoothingAttribute::smooth() {
    core::gfx::Vector2 trend_addition_vector;
    bool x_smooth_now;
    bool y_smooth_now;

    m_vector_frame_count = core::gfx::Vector2(m_vector_frame_count.get_x() + 1, m_vector_frame_count.get_y() + 1);

    x_smooth_now = m_vector_frame_count.get_x() >= m_vector_trend_per_frames.get_x();
    y_smooth_now = m_vector_frame_count.get_y() >= m_vector_trend_per_frames.get_y();

    if(x_smooth_now && y_smooth_now) {
        trend_addition_vector = m_vector_trend;

        m_vector_frame_count = core::gfx::Vector2(0, 0);
    } else if(x_smooth_now) {
        trend_addition_vector = core::gfx::Vector2(m_vector_trend.get_x(), 0);

        m_vector_frame_count = core::gfx::Vector2(0, m_vector_frame_count.get_y());
    } else if(y_smooth_now) {
        trend_addition_vector = core::gfx::Vector2(0, m_vector_trend.get_y());

        m_vector_frame_count = core::gfx::Vector2(m_vector_frame_count.get_x(), 0);
    } else {
        return true;
    }

    switch (get_copy_option()) {
        case AnimationFrameCopyOption::MOSAIC:
            get_target_sprite()->set_mosaic_levels(get_target_sprite()->get_mosaic_levels() + trend_addition_vector);
            break;
        case AnimationFrameCopyOption::POSITION:
            get_target_sprite()->set_position(get_target_sprite()->get_position() + trend_addition_vector);
            break;
        case AnimationFrameCopyOption::SCALE:
            if(get_target_sprite()->is_affine()) {
                get_target_sprite()->set_scale(get_target_sprite()->get_scale() + trend_addition_vector);
            } else {
                return false;
            }
            break;
        default:
            return false;
    }

    return true;
}
