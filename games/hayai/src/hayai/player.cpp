//
// Created by bobby on 04/05/2021.
//

#include "player.hpp"

#include <memory>

hayai::Player::Player(std::shared_ptr<morpheus::core::MainLoop> main_loop,
                      std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> level_background,
                      Level *level) {
    m_player_hud = std::make_unique<PlayerHud>(main_loop);
    m_sprite_base.reset(morpheus::utils::construct_appropriate_sprite_4bpp(false,
                                                                           main_loop->get_blending_controller(),
                                                                           main_loop->get_mosaic_controller()));

    m_current_level = level;
    m_level_background = level_background;

    m_coin_pickup_sfx.reset(morpheus::utils::construct_appropriate_max_mod_sfx(
                                    SFX_COIN_PICKUP, const_cast<void*>(static_cast<const void*>(&soundbank_bin)), 8));
    m_enemy_damage_sfx.reset(morpheus::utils::construct_appropriate_max_mod_sfx(SFX_ENEMY_KILL));
    m_player_damage_sfx.reset(morpheus::utils::construct_appropriate_max_mod_sfx(SFX_PLAYER_HURT));

    m_sprite_base->load_into_palette(playerleftarms0Pal, 32, LEFT_PALETTE_IDS[0]*16);
    m_sprite_base->load_into_palette(playerleftarms1Pal, 32, LEFT_PALETTE_IDS[1]*16);
    m_sprite_base->load_into_palette(playerleftarms2Pal, 32, LEFT_PALETTE_IDS[2]*16);
    m_sprite_base->load_into_palette(playerrightarms0Pal, 32, RIGHT_PALETTE_IDS[0]*16);
    m_sprite_base->load_into_palette(playerrightarms1Pal, 32, RIGHT_PALETTE_IDS[1]*16);
    m_sprite_base->load_into_palette(playerrightarms2Pal, 32, RIGHT_PALETTE_IDS[2]*16);

    for(int i = 0; FLICKER_LENGTH / 60 > i; ++i) {
        m_flicker_animation.emplace_back(morpheus::utils::construct_appropriate_animation_frame(m_sprite_base.get()));

        m_flicker_animation.back()->set_blending_value(16, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
        m_flicker_animation.back()->set_mosaic_levels(morpheus::core::gfx::Vector2(0, 0),
                                                      true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
        m_flicker_animation.back()->set_vblank_delays(30);

        m_flicker_animation.emplace_back(morpheus::utils::construct_appropriate_animation_frame(m_sprite_base.get()));
        m_flicker_animation.back()->set_blending_value(4, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
        m_flicker_animation.back()->set_mosaic_levels(morpheus::core::gfx::Vector2(5, 5), true,
                                                      morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
        m_flicker_animation.back()->set_vblank_delays(30);
    }

    m_game_over_animation.emplace_back(morpheus::utils::construct_appropriate_animation_frame(m_sprite_base.get()));

    m_game_over_animation.back()->set_blending_value(0, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    m_game_over_animation.back()->set_vblank_delays(GAME_OVER_FADE_TIME_FRAMES);

    m_game_over_animation.emplace_back(morpheus::utils::construct_appropriate_animation_frame(m_sprite_base.get()));

    m_game_over_animation.back()->set_blending_value(16, true, morpheus::core::gfx::AnimationSmoothingMode::LINEAR);
    m_game_over_animation.back()->set_vblank_delays(GAME_OVER_FADE_TIME_FRAMES);

    m_game_over_text.reset(morpheus::utils::construct_appropriate_text(false, 3, 2, 28, main_loop.get(), false));


    m_game_over_text_window.reset(morpheus::utils::construct_appropriate_window(
                                                                morpheus::core::gfx::WindowType::WINDOW_1, main_loop));

    m_game_over_text_window->add_background(3);

    m_game_over_text_window->set_window_rect({
        .bottom = 48,
        .left = 80,
        .right = 160,
        .top = 40,
    });

    main_loop->get_blending_controller()->set_blending_mode(morpheus::core::gfx::BlendingMode::USE_WEIGHTS);
    main_loop->get_blending_controller()->set_blend_weight(false, 16);
    main_loop->get_blending_controller()->enable_backdrop_blending(true);

    m_level_background->enable_blending(true);
    m_sprite_base->enable_blending(false);

    m_sprite_base->toggle_mosaic();

    #ifdef _GBA
        auto gba_sprite = static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get());

        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerleftarms0Tiles),
                                    playerleftarms0TilesLen,
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    LEFT_TILE_IDS[0]);
        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerleftarms1Tiles),
                                    playerleftarms1TilesLen,
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    LEFT_TILE_IDS[1]);
        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerleftarms2Tiles),
                                    playerleftarms2TilesLen,morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    LEFT_TILE_IDS[2]);

        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerrightarms2Tiles),
                                    playerrightarms2TilesLen,morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    RIGHT_TILE_IDS[2]);
        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerrightarms1Tiles),
                                    playerrightarms1TilesLen,morpheus::core::gfx::SpriteSize::SIZE_32X32,
                                    RIGHT_TILE_IDS[1]);
        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerrightarms0Tiles),
                                    playerrightarms0TilesLen,
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32,RIGHT_TILE_IDS[0]);
    #elif _NDS
        auto *nds_sprite = static_cast<morpheus::nds::gfx::Sprite*>(m_sprite_base.get());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerleftarms0Tiles),
                                    playerleftarms0TilesLen, LEFT_PALETTE_IDS[0],
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32);
        m_gfx_pointers.push_back(nds_sprite->get_gfx_pointer());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerleftarms1Tiles),
                                    playerleftarms1TilesLen,LEFT_PALETTE_IDS[1],
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32);
        m_gfx_pointers.push_back(nds_sprite->get_gfx_pointer());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerleftarms2Tiles),
                                    playerleftarms2TilesLen,LEFT_PALETTE_IDS[2],
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32);
        m_gfx_pointers.push_back(nds_sprite->get_gfx_pointer());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerrightarms2Tiles),
                                    playerrightarms2TilesLen,RIGHT_PALETTE_IDS[2],
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32);
        m_gfx_pointers.push_back(nds_sprite->get_gfx_pointer());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerrightarms1Tiles),
                                    playerrightarms1TilesLen,RIGHT_PALETTE_IDS[1],
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32);
        m_gfx_pointers.push_back(nds_sprite->get_gfx_pointer());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(playerrightarms0Tiles),
                                    playerrightarms0TilesLen,RIGHT_PALETTE_IDS[0],
                                    morpheus::core::gfx::SpriteSize::SIZE_32X32);
        m_gfx_pointers.push_back(nds_sprite->get_gfx_pointer());
    #endif
}

void hayai::Player::draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    m_sprite_base->draw(obj_attr_buffer, obj_attr_num);
}

void hayai::Player::input(const morpheus::core::InputEvent input_event) {
    if((input_event.state == morpheus::core::InputState::DOWN ||
        input_event.state == morpheus::core::InputState::HELD) && !m_game_over) {
        switch (input_event.button) {
            case morpheus::core::InputButton::A:
                // if the player is not jumping or falling
                if (m_velocity.get_y() == 0 && !m_jumping) {
                    m_current_level->nocash_message("jumping");

                    m_jumping = true;
                    m_jumping_frame = -1;

                    if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                        m_acceleration = morpheus::core::gfx::Vector2(m_velocity.get_x(), std::max(-ACCELERATION_STEP,
                                                                                                   -JUMPING_SPEED));

                        m_target_y_velocity = -JUMPING_SPEED;
                    } else {
                        m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), -JUMPING_SPEED);
                    }
                }

                m_moved_this_frame = true;
                break;
            case morpheus::core::InputButton::DPADLEFT:
                if (m_velocity.get_x() > -static_cast<int>(MAX_SPEED)) {
                    m_moved_this_frame = true;

                    if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                        m_acceleration = morpheus::core::gfx::Vector2(std::max(-REGULAR_SPEED, -ACCELERATION_STEP),
                                                                      m_velocity.get_y());

                        m_target_x_velocity = -REGULAR_SPEED;
                    } else {
                        m_velocity = morpheus::core::gfx::Vector2(-REGULAR_SPEED, m_velocity.get_y());
                    }
                }

                m_left = true;
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                if (m_velocity.get_x() < static_cast<int>(MAX_SPEED)) {
                    m_moved_this_frame = true;

                    if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                        m_acceleration = morpheus::core::gfx::Vector2(std::min(REGULAR_SPEED, ACCELERATION_STEP),
                                                                      m_velocity.get_y());

                        m_target_x_velocity = REGULAR_SPEED;
                    } else {
                        m_velocity = morpheus::core::gfx::Vector2(REGULAR_SPEED, m_velocity.get_y());
                    }
                }

                m_left = false;
                break;
            default:
                break;
        }
    }
}

void hayai::Player::update(const unsigned char cycle_time) {
    if(m_game_over) {
        if(!m_sprite_base->is_playing() && !m_game_over_printed) {
            m_game_over_text->set_print_position(morpheus::core::gfx::Vector2(80, 40));

            m_game_over_text->print("Game over!");

            m_game_over_text_window->enable_window();

            m_game_over_printed = true;
        }

        return;
    }
    //m_current_level->nocash_message("updating");

    if(m_first_run) {
        m_first_run = !m_first_run;

        m_level_background->set_scroll(INITIAL_SCROLL_POSITION);
        m_sprite_base->set_position(INITIAL_SPRITE_POSITION);
    }

    m_player_hud->update_hud_numbers();

    if(m_jumping && m_jumping_frame < 0) {
        m_jumping_frame = cycle_time + 10;

        m_jumping_update_frame = m_jumping_frame % 60;
    } else if(m_jumping && m_jumping_update_frame == cycle_time) {
        m_jumping_frame -= 60;

        if(m_jumping_frame <= 0) {
            m_current_level->nocash_message("no longer jumping due to gravity");

            m_jumping = false;

            if(!ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), 0);
            }
        }
    }

    if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
        if(m_target_x_velocity > m_velocity.get_x()) {
            m_acceleration = morpheus::core::gfx::Vector2(std::min(m_acceleration.get_x() + ACCELERATION_STEP,
                                                                   m_target_x_velocity), m_acceleration.get_y());
        } else if(m_target_x_velocity < m_velocity.get_x()) {
            m_acceleration = morpheus::core::gfx::Vector2(std::max(m_acceleration.get_x() - ACCELERATION_STEP,
                                                                   m_target_x_velocity), m_acceleration.get_y());
        } else if(m_target_y_velocity > m_velocity.get_y()) {
            m_acceleration = morpheus::core::gfx::Vector2(m_acceleration.get_x(),
                                                          std::min(m_acceleration.get_y() + ACCELERATION_STEP,
                                                                   m_target_y_velocity));
        } else if(m_target_y_velocity < m_velocity.get_y()) {
            m_acceleration = morpheus::core::gfx::Vector2(m_acceleration.get_x(),
                                                          std::max(m_acceleration.get_y() - ACCELERATION_STEP,
                                                                      m_target_y_velocity));
        } else {
            m_acceleration = morpheus::core::gfx::Vector2(0, 0);
        }
    } else if(!m_moved_this_frame && !m_jumping) {
        m_velocity = morpheus::core::gfx::Vector2(0, 0);
    }

    if(m_sprite_base->get_position().get_y() + m_level_background->get_scroll().get_y() > 56 * 8) {
        m_velocity = morpheus::core::gfx::Vector2(0, 0);

        m_sprite_base->stop();

        m_level_background->set_scroll(INITIAL_SCROLL_POSITION);
        m_sprite_base->set_position(INITIAL_SPRITE_POSITION);
    }

    m_current_level->nocash_message("applying enemy collision");

    apply_enemy_collision_detection();

    m_current_level->nocash_message("applying x collision");

    apply_x_collision_detection();

    m_current_level->nocash_message("applying y collision");

    apply_y_collision_detection();

    m_current_level->nocash_message("applying animaton");

    if(abs(m_velocity.get_x()) > 0) {
        if(cycle_time == 0) {
            if (m_last_was_left == m_left) {
                if(m_current_animation_frame == 2) {
                    m_current_animation_frame = 1;
                }

                ++m_current_animation_frame;
            } else {
                m_current_animation_frame = 1;
            }
        }

        update_animation(m_left, m_current_animation_frame);
    } else if(abs(m_velocity.get_x()) == 0) {
        update_animation(m_left, 0);
    }

    if(cycle_time % 8 == 0) {
        m_moved_this_frame = false;
    }

    if(ENABLE_ACCEL_MOVEMENT_SYSTEM && m_velocity.get_x() != 0) {
        apply_friction();
    }

    m_current_level->nocash_message("applying speed zones");

    apply_speed_zones();

    m_current_level->nocash_message("applying gravity");

    apply_gravity();

    m_current_level->nocash_message("done!");

    if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
        m_velocity = m_velocity + m_acceleration;
        m_velocity = morpheus::core::gfx::Vector2(std::max(-static_cast<int>(MAX_SPEED),
                                                           std::min(static_cast<int>(MAX_SPEED), m_velocity.get_x())),
                                                  std::max(-static_cast<int>(MAX_SPEED),
                                                           std::min(static_cast<int>(MAX_SPEED), m_velocity.get_y())));
    }

    m_last_was_left = m_left;
    m_past_velocity = m_velocity;
}

void hayai::Player::update_animation(bool left, unsigned int animation_frame) {
    animation_frame = std::min(animation_frame, 2u);

    if(left) {
        #ifdef _GBA
            static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->build_attr2(
                    LEFT_PALETTE_IDS[animation_frame], LEFT_TILE_IDS[animation_frame]);
        #elif _NDS
            auto *nds_sprite = static_cast<morpheus::nds::gfx::Sprite*>(m_sprite_base.get());

            nds_sprite->set_gfx_pointer(m_gfx_pointers[animation_frame]);
            nds_sprite->set_palette_id(LEFT_PALETTE_IDS[animation_frame]);
        #endif
    } else {
        #ifdef _GBA
            static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->build_attr2(
                    RIGHT_PALETTE_IDS[animation_frame], RIGHT_TILE_IDS[animation_frame]);
        #elif _NDS
            auto *nds_sprite = static_cast<morpheus::nds::gfx::Sprite*>(m_sprite_base.get());

            nds_sprite->set_gfx_pointer(m_gfx_pointers[5 - animation_frame]);
            nds_sprite->set_palette_id(LEFT_PALETTE_IDS[animation_frame]);
        #endif
    }
}

bool hayai::Player::coin_tile_index(unsigned int tile_index) {
    for(unsigned int coin_tile_index : m_current_level->get_current_coin_indices()) {
        if(coin_tile_index == tile_index) {
            return true;
        }
    }

    return false;
}

bool hayai::Player::collision_tile_id(unsigned int tile_id) {
    for(unsigned int collision_tile_id : m_current_level->get_no_collision_tile_ids()) {
        if(collision_tile_id == tile_id) {
            return false;
        }
    }

    return true;
}

bool hayai::Player::friction_tile_id(unsigned int tile_id) {
    for(unsigned int friction_tile_id : m_current_level->get_friction_tile_ids()) {
        if(friction_tile_id == tile_id) {
            return true;
        }
    }

    return false;
}

hayai::Player::SpeedZone hayai::Player::get_speed_zone() const {
    morpheus::core::gfx::Vector2 player_position = m_sprite_base->get_position();
    SpeedZone return_value = SpeedZone::NO_EFFECT;
    std::vector<morpheus::core::gfx::Vector2> tile_positions;//

    for(int y = 0; PLAYER_SIZE.get_y() > y; ++y) {
        tile_positions.push_back(player_position + morpheus::core::gfx::Vector2(0, y));
    }

    for(morpheus::core::gfx::Vector2 &tile_position : tile_positions) {
        unsigned int tile_id = m_level_background->get_tile_id_at_position(tile_position);

        for(unsigned int speed_left_tile_id : Level::SPEED_LEFT_TILES) {
            if(tile_id == speed_left_tile_id) {
                if(return_value == SpeedZone::SPEED_RIGHT) {
                    return_value = SpeedZone::NO_EFFECT;
                } else {
                    return_value = SpeedZone::SPEED_LEFT;
                }

                break;
            }
        }

        for(unsigned int speed_right_tile_id : Level::SPEED_RIGHT_TILES) {
            if(tile_id == speed_right_tile_id) {
                if(return_value == SpeedZone::SPEED_LEFT) {
                    return_value = SpeedZone::NO_EFFECT;
                } else {
                    return_value = SpeedZone::SPEED_RIGHT;
                }

                break;
            }
        }
    }

    return return_value;
}

void hayai::Player::apply_enemy_collision_detection() {
    for(unsigned int i = 0; m_enemies.size() > i; ++i) {
        m_current_level->nocash_message("getting positions");

        morpheus::core::gfx::Vector2 enemy_position = m_enemies[i]->get_sprite()->get_position();
        morpheus::core::gfx::Vector2 player_position = get_sprite()->get_position();

        m_current_level->nocash_message("checking jumping");

        if(m_enemies[i]->is_jumping()) {
            enemy_position = enemy_position + Enemy::JUMPING_OFFSET;
        } else {
            enemy_position = enemy_position + Enemy::REGULAR_OFFSET;
        }

        m_current_level->nocash_message("collision checking");

        if(player_position.get_x() < enemy_position.get_x() + Enemy::ENEMY_SIZE.get_x() &&
           player_position.get_x() + 32 > enemy_position.get_x() &&
           player_position.get_y() < player_position.get_y() + Enemy::ENEMY_SIZE.get_y() &&
           player_position.get_y() + 32 > enemy_position.get_y() && player_position.get_x()) {
            if(player_position.get_y() < enemy_position.get_y() - (Enemy::ENEMY_SIZE.get_y() + 2)) {
                m_current_level->nocash_message("starting effect");

                if(m_enemy_damage_sfx != nullptr) {
                    m_enemy_damage_sfx->start_effect(false);
                }

                m_current_level->nocash_message("killing enemy");

                m_current_level->kill_enemy(m_enemies[i]);

                m_current_level->nocash_message("erasing enemy");

                m_enemies.erase(m_enemies.begin() + i);

                m_current_level->nocash_message("hit enemy!");
            } else if(!m_sprite_base->is_playing()) {
                m_current_level->nocash_message("getting health number");

                int current_health_number = m_player_hud->get_health_number();

                if(current_health_number > 0) {
                    m_current_level->nocash_message("starting effect");

                    if(m_player_damage_sfx != nullptr) {
                        m_player_damage_sfx->start_effect(false);
                    }

                    m_current_level->nocash_message("setting stop frame");

                    m_sprite_base->set_stop_frame(0);

                    m_current_level->nocash_message("setting health number");

                    m_player_hud->set_health_number(m_player_hud->get_health_number() - 1);

                    m_current_level->nocash_message("setting frames");
                    m_sprite_base->set_frames(m_flicker_animation);
                } else {
                    // game over!
                    m_game_over = true;

                    m_current_level->nocash_message("game overing");

                    m_sprite_base->get_blending_controller()->set_blending_mode(morpheus::core::gfx::BlendingMode::FADE_TO_BLACK);
                    m_sprite_base->get_blending_controller()->enable_backdrop_blending(false);
                    m_sprite_base->get_blending_controller()->enable_background_blending(false, 0);

                    m_level_background->enable_blending(false);

                    m_sprite_base->set_stop_frame(1);

                    m_sprite_base->set_frames(m_game_over_animation);
                }

                m_sprite_base->play(false);
            }
        }
    }
}

void hayai::Player::apply_friction() {
    morpheus::core::gfx::Vector2 player_position = m_sprite_base->get_position();

    auto tile_pos_1 = morpheus::core::gfx::Vector2(player_position.get_x(),
                                                   player_position.get_y() + PLAYER_SIZE.get_y());
    morpheus::core::gfx::Vector2 tile_pos_2 = player_position + (PLAYER_SIZE / morpheus::core::gfx::Vector2(2, 1));

    unsigned int tile_id_1 = m_level_background->get_tile_id_at_position(tile_pos_1);
    unsigned int tile_id_2 = m_level_background->get_tile_id_at_position(tile_pos_2);

    if(friction_tile_id(tile_id_1) || friction_tile_id(tile_id_2)) {
        if(m_velocity.get_x() > 0) {
            m_velocity = m_velocity - morpheus::core::gfx::Vector2(FRICTION, 0);

            if(m_velocity.get_x() < 0) {
                m_velocity = morpheus::core::gfx::Vector2(0, 0);
            }
        } else if(m_velocity.get_x() < 0) {
            m_velocity = m_velocity + morpheus::core::gfx::Vector2(FRICTION, 0);

            if(m_velocity.get_x() > 0) {
                m_velocity = morpheus::core::gfx::Vector2(0, 0);
            }
        }
    }
}

void hayai::Player::apply_gravity() {
    morpheus::core::gfx::Vector2 player_position = m_sprite_base->get_position() + m_velocity;

    auto tile_pos_1 = morpheus::core::gfx::Vector2(player_position.get_x(),
                                                   player_position.get_y() + PLAYER_SIZE.get_y());
    morpheus::core::gfx::Vector2 tile_pos_2 = player_position + (PLAYER_SIZE / morpheus::core::gfx::Vector2(2, 1));

    unsigned int tile_id_1 = m_level_background->get_tile_id_at_position(tile_pos_1);
    unsigned int tile_id_2 = m_level_background->get_tile_id_at_position(tile_pos_2);

    if(!m_jumping) {
        if(collision_tile_id(tile_id_1) || collision_tile_id(tile_id_2)) {
            bool coin_tile_1 = coin_tile_index(tile_id_1);
            bool coin_tile_2 = coin_tile_index(tile_id_2);

            if(coin_tile_1) {
                if(m_coin_pickup_sfx != nullptr) {
                    m_coin_pickup_sfx->start_effect(false);
                }

                remove_coin_at_position(tile_pos_1);
            }

            if(coin_tile_2) {
                if(m_coin_pickup_sfx != nullptr) {
                    m_coin_pickup_sfx->start_effect(false);
                }

                remove_coin_at_position(tile_pos_2);
            }

            if(!coin_tile_1 || !coin_tile_2) {
                m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), 0);
            }
        } else {
            if(m_velocity.get_y() < GRAVITY) {
                if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                    m_acceleration = morpheus::core::gfx::Vector2(m_acceleration.get_x(),
                                                                  std::max(m_acceleration.get_y() + GRAVITY, GRAVITY));
                } else {
                    m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), m_velocity.get_y() + GRAVITY);
                }
            }
        }
    }
}

void hayai::Player::apply_speed_zones() {
    SpeedZone current_speed_zone = get_speed_zone();

    switch (current_speed_zone) {
        case SpeedZone::NO_EFFECT:
            break;
        case SpeedZone::SPEED_LEFT:
            if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                m_acceleration = morpheus::core::gfx::Vector2(-SPEED_ZONE_ACCEL, 0);
            } else {
                m_velocity = morpheus::core::gfx::Vector2(-SPEED_ZONE_ACCEL, 0);
            }
            break;
        case SpeedZone::SPEED_RIGHT:
            if(ENABLE_ACCEL_MOVEMENT_SYSTEM) {
                m_acceleration = morpheus::core::gfx::Vector2(SPEED_ZONE_ACCEL, 0);
            } else {
                m_velocity = m_velocity + morpheus::core::gfx::Vector2(SPEED_ZONE_ACCEL, 0);
            }
            break;
    }
}

void hayai::Player::apply_x_collision_detection() {
    if(m_velocity.get_x() != 0) {
        for(int y = 0; PLAYER_SIZE.get_y() > y; y += 8) {
            morpheus::core::gfx::Vector2 collision_position = m_sprite_base->get_position();
            unsigned int current_tile_index;

            if(m_velocity.get_x() > 0) {
                collision_position = collision_position + morpheus::core::gfx::Vector2(
                        PLAYER_SIZE.get_x() + m_velocity.get_x(), m_velocity.get_y() + y);
            } else if(m_velocity.get_x() < 0) {
                collision_position = collision_position + morpheus::core::gfx::Vector2(m_velocity.get_x(),
                                                                                       m_velocity.get_y() + y);
            }

            while(m_velocity.get_x() != 0) {
                current_tile_index = m_level_background->get_tile_index_at_position(collision_position);

                if(collision_tile_id(m_level_background->get_tile_id_at_index(current_tile_index))) {
                    if(coin_tile_index(current_tile_index)) {
                        if(m_coin_pickup_sfx != nullptr) {
                            m_coin_pickup_sfx->start_effect(false);
                        }

                        remove_coin_at_position(collision_position);
                    } else {
                        morpheus::core::gfx::Vector2 vector_difference;

                        if (m_velocity.get_x() > 0) {
                            vector_difference = morpheus::core::gfx::Vector2(-1, 0);
                        } else if (m_velocity.get_x() < 0) {
                            vector_difference = morpheus::core::gfx::Vector2(1, 0);
                        }

                        collision_position = collision_position + vector_difference;
                        m_velocity = m_velocity + vector_difference;
                    }
                } else {
                    break;
                }
            }
        }
    }
}

void hayai::Player::apply_y_collision_detection() {
    if(m_velocity.get_y() < 0) {
        for(int x = 0; PLAYER_SIZE.get_x() > x; x += 8) {
            morpheus::core::gfx::Vector2 collision_position = m_sprite_base->get_position() + m_velocity +
                                                              morpheus::core::gfx::Vector2(x, 0);
            unsigned int current_tile_index;

            while(m_velocity.get_y() != 0) {
                current_tile_index = m_level_background->get_tile_index_at_position(collision_position);

                if(collision_tile_id(m_level_background->get_tile_id_at_index(current_tile_index))) {
                    if(coin_tile_index(current_tile_index)) {
                        if(m_coin_pickup_sfx != nullptr) {
                            m_coin_pickup_sfx->start_effect(false);
                        }

                        remove_coin_at_position(collision_position);
                    } else {
                        collision_position = collision_position + morpheus::core::gfx::Vector2(0, 1);
                        m_velocity = m_velocity + morpheus::core::gfx::Vector2(0, 1);
                    }
                } else {
                    break;
                }
            }

            if(m_velocity.get_y() == 0) {
                m_current_level->nocash_message("no longer jumping due to collision");

                m_jumping = false;
                break;
            }
        }
    }
}

void hayai::Player::remove_coin_at_position(morpheus::core::gfx::Vector2 position) {
    unsigned int coin_tile_offset = 0;
    bool found_offset = false;
    std::array<int, 4> tile_removal_indices = {m_level_background->get_tile_index_at_position(position), 0, 0, 0};
    std::array<morpheus::core::gfx::Vector2, 3> tile_removal_positions;

    unsigned int init_coin_tile_id = m_level_background->get_tile_id_at_index(tile_removal_indices[0]);

    for(unsigned int i = 0; Level::DARK_COIN_TILES.size() > i; ++i) {
        if(init_coin_tile_id == Level::DARK_COIN_TILES[i]) {
            coin_tile_offset = i;
            found_offset = true;
            break;
        }
    }

    if(!found_offset) {
        for (unsigned int i = 0; Level::LIT_COIN_TILES.size() > i; ++i) {
            if (init_coin_tile_id == Level::LIT_COIN_TILES[i]) {
                coin_tile_offset = i;
            }
        }
    }

    switch (coin_tile_offset) {
        case 0:
            tile_removal_positions = {
                    morpheus::core::gfx::Vector2(8, 0),
                    morpheus::core::gfx::Vector2(0, 8),
                    morpheus::core::gfx::Vector2(8, 8)
            };
            break;
        case 1:
            tile_removal_positions = {
                    morpheus::core::gfx::Vector2(-8, 0),
                    morpheus::core::gfx::Vector2(-8, 8),
                    morpheus::core::gfx::Vector2(0, 8)
            };
            break;
        case 2:
            tile_removal_positions = {
                    morpheus::core::gfx::Vector2(0, -8),
                    morpheus::core::gfx::Vector2(8, -8),
                    morpheus::core::gfx::Vector2(8, 0)
            };
            break;
        case 3:
            tile_removal_positions = {
                    morpheus::core::gfx::Vector2(-8, -8),
                    morpheus::core::gfx::Vector2(0, -8),
                    morpheus::core::gfx::Vector2(-8, 0)
            };
            break;
    }

    m_player_hud->set_coin_number(m_player_hud->get_coin_number() + 1);


    for(unsigned int i = 1; tile_removal_indices.size() > i; ++i) {
        morpheus::core::gfx::Vector2 new_position = position + tile_removal_positions[i - 1];

        tile_removal_indices[i] = m_level_background->get_tile_index_at_position(new_position);
    }

    if(m_current_level != nullptr) {
        m_current_level->delete_coin_indices(tile_removal_indices);
    }
}

