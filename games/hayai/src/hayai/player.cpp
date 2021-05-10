//
// Created by bobby on 04/05/2021.
//

#include "player.hpp"

hayai::Player::Player(std::shared_ptr<morpheus::core::MainLoop> main_loop,
                      std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> level_background,
                      Level *level) {
    m_sprite_base.reset(morpheus::utils::construct_appropriate_sprite_4bpp(false,
                                                                           main_loop->get_blending_controller(),
                                                                           main_loop->get_mosaic_controller()));

    level_background->set_scroll(morpheus::core::gfx::Vector2(32*8, 30*8));

    m_current_level = level;
    m_level_background = level_background;

    m_sprite_base->set_position(88, 48);

    m_sprite_base->load_into_palette(playerleftarms0Pal, 32, LEFT_PALETTE_IDS[0]*16);
    m_sprite_base->load_into_palette(playerleftarms1Pal, 32, LEFT_PALETTE_IDS[1]*16);
    m_sprite_base->load_into_palette(playerleftarms2Pal, 32, LEFT_PALETTE_IDS[2]*16);
    m_sprite_base->load_into_palette(playerrightarms0Pal, 32, RIGHT_PALETTE_IDS[0]*16);
    m_sprite_base->load_into_palette(playerrightarms1Pal, 32, RIGHT_PALETTE_IDS[1]*16);
    m_sprite_base->load_into_palette(playerrightarms2Pal, 32, RIGHT_PALETTE_IDS[2]*16);

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(playerleftarms0Tiles),
                playerleftarms0TilesLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32,
                LEFT_TILE_IDS[0]);
        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(playerleftarms1Tiles),
                playerleftarms1TilesLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32,
                LEFT_TILE_IDS[1]);
        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array(
            reinterpret_cast<const unsigned short *>(playerleftarms2Tiles),
            playerleftarms2TilesLen,
            morpheus::core::gfx::SpriteSize::SIZE_32X32,
            LEFT_TILE_IDS[2]);

        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(playerrightarms2Tiles),
                playerrightarms2TilesLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32,
                RIGHT_TILE_IDS[2]);
        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(playerrightarms1Tiles),
                playerrightarms1TilesLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32,
                RIGHT_TILE_IDS[1]);
        static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->load_from_array(
                reinterpret_cast<const unsigned short *>(playerrightarms0Tiles),
                playerrightarms0TilesLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32,
                RIGHT_TILE_IDS[0]);
    #elif _NDS
    #endif
}

void hayai::Player::draw(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) {
    m_sprite_base->draw(obj_attr_buffer, obj_attr_num);
}

void hayai::Player::input(const morpheus::core::InputEvent input_event) {
    if(input_event.state == morpheus::core::InputState::DOWN || input_event.state == morpheus::core::InputState::UP) {
        switch (input_event.button) {
            case morpheus::core::InputButton::A:
                // if the player is not jumping or falling
                if(m_velocity.get_y() == 0) {
                    //
                }

                m_moved_this_frame = true;
                break;
            case morpheus::core::InputButton::DPADLEFT:
                if(m_velocity.get_x() > -static_cast<int>(MAX_SPEED)) {
                    m_acceleration = morpheus::core::gfx::Vector2(m_acceleration.get_x() - ACCELERATION_STEP,
                                                                  m_acceleration.get_y());
                }

                m_moved_this_frame = true;
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                if(m_velocity.get_x() < static_cast<int>(MAX_SPEED)) {
                    m_acceleration = morpheus::core::gfx::Vector2(m_acceleration.get_x() + ACCELERATION_STEP,
                                                                  m_acceleration.get_y());
                }

                m_moved_this_frame = true;
                break;
            default:
                break;
        }
    }
}

void hayai::Player::update(const unsigned char cycle_time) {
    morpheus::core::gfx::Vector2 past_velocity = m_velocity;

    if(!m_moved_this_frame) {
        if(m_velocity == morpheus::core::gfx::Vector2(0, 0)) {
            m_acceleration = morpheus::core::gfx::Vector2(0, 0);
        } else {
            int new_x_accel = m_acceleration.get_x();
            int new_y_accel = m_acceleration.get_y();

            if(new_x_accel < 0) {
                new_x_accel += FRICTION;
            } else if(new_x_accel > 0) {
                new_x_accel -= FRICTION;
            }

            m_acceleration = morpheus::core::gfx::Vector2(new_x_accel, new_y_accel);
        }
    }

    m_velocity = morpheus::core::gfx::Vector2(std::max(std::min(m_velocity.get_x() + m_acceleration.get_x(),
                                                          static_cast<int>(MAX_SPEED)), -static_cast<int>(MAX_SPEED)),
                                              std::max(std::min(m_velocity.get_y() + m_acceleration.get_y(),
                                                                static_cast<int>(MAX_SPEED)),
                                                       -static_cast<int>(MAX_SPEED)));

    if(!m_moved_this_frame) {
        if((past_velocity.get_x() > 0 && m_velocity.get_x() < 0) ||
           (past_velocity.get_x() < 0 && m_velocity.get_x() > 0)) {
            m_velocity = morpheus::core::gfx::Vector2(0, past_velocity.get_y());
        }

        if((past_velocity.get_y() > 0 && m_velocity.get_y() < 0) ||
           (past_velocity.get_y() < 0 && m_velocity.get_y() > 0)) {
            m_velocity = morpheus::core::gfx::Vector2(past_velocity.get_x(), 0);
        }
    }

    //if(m_velocity.get_y() + )

    restrict_velocity(true, m_velocity.get_x() >= 0);
    restrict_velocity(false, m_velocity.get_y() >= 0);

    //m_sprite_base->set_position(m_sprite_base->get_position() + m_velocity);

    if(m_velocity.get_x() > 0) {
        if(cycle_time == 30 || cycle_time == 0) {
            if (m_past_velocity.get_x() > 0) {
                if(m_current_animation_frame == 2) {
                    m_current_animation_frame = 0;
                }

                ++m_current_animation_frame;
            } else {
                m_current_animation_frame = 0;
            }
        }

        update_animation(false, m_current_animation_frame);
    } else if(m_velocity.get_x() < 0) {
        if(m_past_velocity.get_x() < 0) {
            if(cycle_time == 30 || cycle_time == 0) {
                if(m_current_animation_frame == 2) {
                    m_current_animation_frame = 0;
                }

                ++m_current_animation_frame;
            }
        } else {
            m_current_animation_frame = 0;
        }

        update_animation(true, m_current_animation_frame);
    } else if(m_velocity.get_x() == 0) {
        update_animation(m_last_was_left, 0);
    }

    m_last_was_left = m_velocity.get_x() < 0;
    m_moved_this_frame = false;
    m_past_velocity = m_velocity;
}

void hayai::Player::update_animation(bool left, unsigned int animation_frame) {
    if(left) {
        #ifdef _GBA
            static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->build_attr2(
                    LEFT_PALETTE_IDS[animation_frame], LEFT_TILE_IDS[animation_frame]);
        #elif _NDS
        #endif
    } else {
        #ifdef _GBA
            static_cast<morpheus::gba::gfx::Sprite*>(m_sprite_base.get())->build_attr2(
                    RIGHT_PALETTE_IDS[animation_frame], RIGHT_TILE_IDS[animation_frame]);
        #elif _NDS
        #endif
    }
}

bool hayai::Player::collision_tile_id(unsigned int tile_id) {
    for(unsigned int collision_tile_id : m_current_level->get_collision_tile_ids()) {
        if(collision_tile_id == tile_id) {
            return true;
        }
    }

    return false;
}

void hayai::Player::restrict_velocity(bool x, bool pos) {
    if(m_current_level != nullptr && m_level_background != nullptr) {
        std::vector<unsigned int> tile_ids_to_check;
        std::vector<morpheus::core::gfx::Vector2> positions;

        int velocity_value;

        if(x) {
            velocity_value = abs(m_velocity.get_x());
        } else {
            velocity_value = abs(m_velocity.get_y());
        }

        //nocash_puts(std::string("velocity value is: " + std::to_string(velocity_value)).c_str());

        if(velocity_value == 0) {
            return;
        }

        for(int i = 0; velocity_value > i; i += 8) {
            morpheus::core::gfx::Vector2 position;

            if(x) {
                if(pos) {
                    position = m_sprite_base->get_position() + morpheus::core::gfx::Vector2(i, 0);
                } else {
                    position = m_sprite_base->get_position() - morpheus::core::gfx::Vector2(i, 0);
                }
            } else {
                if(pos) {
                    position = m_sprite_base->get_position() + morpheus::core::gfx::Vector2(0, i);
                } else {
                    position = m_sprite_base->get_position() - morpheus::core::gfx::Vector2(0, i);
                }
            }


            positions.push_back(position);
            tile_ids_to_check.push_back(m_level_background->get_tile_id_at_position(position));
        }

        for(int i = 0; static_cast<int>(tile_ids_to_check.size()) > i; ++i) {
            bool velocity_restricted = false;

            if(collision_tile_id(tile_ids_to_check[i])) {
                nocash_puts(std::string("tile id " + std::to_string(tile_ids_to_check[i]) +
                                                                        " matches").c_str());
                nocash_puts(std::string("at " +
                                            (m_level_background->get_scroll() + positions[i]).to_string()).c_str());

                if(x) {
                    if(pos) {
                        m_velocity = morpheus::core::gfx::Vector2(0,//std::min(8 * (i-1), m_velocity.get_x()),
                                                                  m_velocity.get_y());
                    } else {
                        m_velocity = morpheus::core::gfx::Vector2(0,//std::max(-8 * (i-1), m_velocity.get_x()),
                                                                  m_velocity.get_y());
                    }
                } else {
                    if(pos) {
                        if(is_player_collided_with(m_sprite_base->get_position(), positions[i])) {
                            morpheus::core::gfx::Vector2 working_position = positions[i];
                            morpheus::core::gfx::Vector2 last_collision_position = positions[i];

                            while(true) {
                                working_position = morpheus::core::gfx::Vector2(working_position.get_x(),
                                                                                working_position.get_y() - 8);

                                if(!is_player_collided_with(last_collision_position, working_position) &&
                                   !collision_tile_id(m_level_background->get_tile_id_at_position(working_position))) {
                                    nocash_puts(std::string("old position: " + positions[i].to_string()).c_str());
                                    nocash_puts(std::string("new position: " + working_position.to_string()).c_str());

                                    m_sprite_base->set_position(working_position);

                                    break;
                                } else if(collision_tile_id(m_level_background->
                                          get_tile_id_at_position(working_position))) {
                                    last_collision_position = working_position;
                                }
                            }
                        }

                        m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), 0);
                                                                  //std::min(8 * (i-1), m_velocity.get_y()));
                    } else {
                        m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), 0);
                                                                  //std::max(-8 * (i-1), m_velocity.get_y()));
                    }
                }

                velocity_restricted = true;

                break;
            }


            if(velocity_restricted) {
                nocash_puts("velocity was restricted");
                nocash_puts(std::string("new velocity is " + m_velocity.to_string()).c_str());

                break;
            }
        }
    }
}
