//
// Created by bobby on 26/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_TEST_ANIMATION_HPP
#define MORPHEUS_GBA_TEST_TEST_ANIMATION_HPP

#ifdef _GBA
#include <morpheus/gba/gfx/gba_animation_frame.hpp>
#elif _NDS
#endif

#include <utils.hpp>

std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> get_animations(
                                                                        morpheus::core::gfx::SpriteBase *sprite_base) {
    std::vector<std::shared_ptr<morpheus::core::gfx::AnimationFrame>> animation_frames;

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
                            morpheus::utils::construct_appropriate_animation_frame(sprite_base)));

    animation_frames[0]->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 1));
    animation_frames[0]->set_position(morpheus::core::gfx::Vector2(50, 50), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[0]->set_vblank_delays(60u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[0].get())));

    animation_frames[1]->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 1));
    animation_frames[1]->set_position(morpheus::core::gfx::Vector2(100, 10), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[1]->set_vblank_delays(60u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[1].get())));

    animation_frames[2]->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 5));
    animation_frames[2]->set_position(morpheus::core::gfx::Vector2(10, 100), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[2]->set_vblank_delays(60u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[2].get())));

    animation_frames[3]->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 5));
    animation_frames[3]->set_position(morpheus::core::gfx::Vector2(100, 100), true,
                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    animation_frames[3]->set_vblank_delays(60u);

    animation_frames.push_back(std::shared_ptr<morpheus::core::gfx::AnimationFrame>(
            morpheus::utils::construct_appropriate_animation_frame(sprite_base, animation_frames[2].get())));

    animation_frames[4]->set_mosaic_levels(morpheus::core::gfx::Vector2(1, 1));
    animation_frames[4]->set_position(morpheus::core::gfx::Vector2(0, 0));
    animation_frames[4]->set_vblank_delays(60u);

    return animation_frames;
}

#endif //MORPHEUS_GBA_TEST_TEST_ANIMATION_HPP
