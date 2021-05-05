//
// Created by bobby on 26/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_TEST_ANIMATION_HPP
#define MORPHEUS_GBA_TEST_TEST_ANIMATION_HPP

#include <utils.hpp>

std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> get_animations(
                                                                        morpheus::core::gfx::SpriteBase *sprite_base) {
    std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> animation_frames;

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
                            morpheus::utils::construct_appropriate_animation_frame(sprite_base)));

    animation_frames[0]->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 1), true,
                                           morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[0]->set_position(morpheus::core::gfx::Vector2(50, 50), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[0]->set_rotation(0, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    /*animation_frames[0]->set_scale(morpheus::core::gfx::Vector2(1 << 8, 1 << 8), true,
                                   morpheus::core::gfx::AnimationSmoothingMode::LINEAR);*/
    animation_frames[0]->set_vblank_delays(120u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[0].get())));

    animation_frames[1]->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 1), true,
                                           morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[1]->set_position(morpheus::core::gfx::Vector2(100, 10), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[1]->set_rotation(1024, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    /*animation_frames[1]->set_scale(morpheus::core::gfx::Vector2(1 << 4, 1 << 4), true,
                                   morpheus::core::gfx::AnimationSmoothingMode::LINEAR);*/
    animation_frames[1]->set_vblank_delays(120u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[1].get())));

    animation_frames[2]->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 5), true,
                                           morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[2]->set_position(morpheus::core::gfx::Vector2(10, 100), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[2]->set_rotation(4096, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    /*animation_frames[2]->set_scale(morpheus::core::gfx::Vector2(1 << 4, 1 << 4), true,
                                   morpheus::core::gfx::AnimationSmoothingMode::LINEAR);*/
    animation_frames[2]->set_vblank_delays(120u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[2].get())));

    animation_frames[3]->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 5), true,
                                           morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[3]->set_position(morpheus::core::gfx::Vector2(100, 100), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[3]->set_rotation(2048, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    /*animation_frames[3]->set_scale(morpheus::core::gfx::Vector2(1 << 8, 1 << 8), true,
                                   morpheus::core::gfx::AnimationSmoothingMode::LINEAR);*/
    animation_frames[3]->set_vblank_delays(120u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[2].get())));

    animation_frames[4]->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 1), true,
                                           morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[4]->set_position(morpheus::core::gfx::Vector2(0, 0), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[4]->set_rotation(1024, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    /*animation_frames[4]->set_scale(morpheus::core::gfx::Vector2(1 << 8, 1 << 8), true,
                                   morpheus::core::gfx::AnimationSmoothingMode::LINEAR);*/
    animation_frames[4]->set_vblank_delays(120u);

    return animation_frames;
}

#endif //MORPHEUS_GBA_TEST_TEST_ANIMATION_HPP
