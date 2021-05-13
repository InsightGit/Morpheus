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

    m_current_level = level;
    m_level_background = level_background;

    m_level_background->set_scroll(INITIAL_SCROLL_POSITION);
    m_sprite_base->set_position(INITIAL_SPRITE_POSITION);

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
    if(input_event.state == morpheus::core::InputState::DOWN || input_event.state == morpheus::core::InputState::HELD) {
        switch (input_event.button) {
            case morpheus::core::InputButton::A:
                // if the player is not jumping or falling
                if (m_velocity.get_y() == 0 && !m_jumping) {
                    nocash_puts("jumping triggered");

                    m_jumping = true;
                    m_jumping_frame = -1;
                    m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), -JUMPING_SPEED);
                }

                m_moved_this_frame = true;
                break;
            case morpheus::core::InputButton::DPADLEFT:
                if (m_velocity.get_x() > -static_cast<int>(MAX_SPEED)) {
                    m_moved_this_frame = true;
                    m_velocity = morpheus::core::gfx::Vector2(-REGULAR_SPEED, m_velocity.get_y());

                    /*auto proposed_velocity = morpheus::core::gfx::Vector2(-REGULAR_SPEED, 0);

                    if(!collision_tile_id(m_level_background->get_tile_id_at_position(
                            m_sprite_base->get_position() + proposed_velocity))) {
                        m_velocity = proposed_velocity;

                        m_moved_this_frame = true;
                    } else {
                        nocash_puts(std::string("left collision at " +
                                                    (m_sprite_base->get_position() + proposed_velocity).to_string()).
                                                    c_str());

                        while(m_velocity.get_x() < 0) {
                            morpheus::core::gfx::Vector2 current_collision_position = m_sprite_base->get_position() +
                                                                                      proposed_velocity;

                            current_collision_position = current_collision_position +
                                                         morpheus::core::gfx::Vector2(1, 0);

                            if(collision_tile_id(m_level_background->
                                    get_tile_id_at_position(current_collision_position))) {
                                m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x() + 1, m_velocity.get_y());
                            } else {
                                m_moved_this_frame = true;
                                break;
                            }
                        }
                    }*/
                }

                m_left = true;
                break;
            case morpheus::core::InputButton::DPADRIGHT:
                if (m_velocity.get_x() < static_cast<int>(MAX_SPEED)) {
                    m_moved_this_frame = true;
                    m_velocity = morpheus::core::gfx::Vector2(REGULAR_SPEED, m_velocity.get_y());
                    /*auto proposed_velocity = morpheus::core::gfx::Vector2(REGULAR_SPEED, 0);
                    auto sprite_width_vector = morpheus::core::gfx::Vector2(PLAYER_SIZE.get_x(), 0);

                    if(!collision_tile_id(m_level_background->get_tile_id_at_position(
                            m_sprite_base->get_position() + sprite_width_vector + proposed_velocity))) {
                        m_velocity = proposed_velocity;
                        m_moved_this_frame = true;
                    } else {
                        morpheus::core::gfx::Vector2 current_collision_position = m_sprite_base->get_position() +
                                                                                  sprite_width_vector +
                                                                                  proposed_velocity;

                        while(m_velocity.get_x() > 0) {
                            current_collision_position = current_collision_position -
                                                         morpheus::core::gfx::Vector2(1, 0);

                            if(collision_tile_id(m_level_background->
                                                 get_tile_id_at_position(current_collision_position))) {
                                m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x() - 1, m_velocity.get_y());
                            } else {
                                m_moved_this_frame = true;
                                break;
                            }
                        }

                        nocash_puts(std::string("right collision at " +
                                                    (m_sprite_base->get_position() + sprite_width_vector + proposed_velocity).to_string()).
                                                    c_str());
                    }*/
                }

                m_left = false;
                break;
            default:
                break;
        }
    }
}

void hayai::Player::update(const unsigned char cycle_time) {
    if(m_jumping && m_jumping_frame < 0) {
        m_jumping_frame = cycle_time;
        m_jumping_update_frame = m_jumping_frame % 60;
    } else if(m_jumping && m_jumping_update_frame == cycle_time) {
        m_jumping_frame -= 60;

        if(m_jumping_frame <= 0) {
            m_jumping = false;

            m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), 0);
        }
    }

    if(!m_moved_this_frame) {
        m_velocity = morpheus::core::gfx::Vector2(0, 0);
    }

    if(m_sprite_base->get_position().get_y() + m_level_background->get_scroll().get_y() > 56 * 8) {
        m_velocity = morpheus::core::gfx::Vector2(0, 0);

        m_level_background->set_scroll(INITIAL_SCROLL_POSITION);
        m_sprite_base->set_position(INITIAL_SPRITE_POSITION);
    }

    if(m_velocity.get_x() != 0) {
        for(int y = 0; PLAYER_SIZE.get_y() > y; y += 8) {
            morpheus::core::gfx::Vector2 collision_position = m_sprite_base->get_position();

            if(m_velocity.get_x() > 0) {
                collision_position = collision_position + morpheus::core::gfx::Vector2(
                                                PLAYER_SIZE.get_x() + m_velocity.get_x(), y);
            } else if(m_velocity.get_x() < 0) {
                collision_position = collision_position + morpheus::core::gfx::Vector2(
                        -PLAYER_SIZE.get_x() + m_velocity.get_x(), -y);
            }

            while(m_velocity.get_x() != 0) {
                if(collision_tile_id(m_level_background->
                        get_tile_id_at_position(collision_position))) {
                    morpheus::core::gfx::Vector2 vector_difference;


                    if(m_velocity.get_x() > 0) {
                        vector_difference = morpheus::core::gfx::Vector2(-1, 0);
                    } else if(m_velocity.get_x() < 0) {
                       vector_difference = morpheus::core::gfx::Vector2(1, 0);
                    }

                    collision_position = collision_position + vector_difference;
                    m_velocity = m_velocity + vector_difference;
                } else {
                    break;
                }
            }

            /*if(collision_tile_id(m_level_background->get_tile_id_at_position(collision_position))) {
                m_velocity = morpheus::core::gfx::Vector2(0, m_velocity.get_y());
            }*/
        }
    }

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

    apply_gravity();

    m_last_was_left = m_left;
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

void hayai::Player::apply_gravity() {
    auto player_position = m_sprite_base->get_position();
    unsigned int tile_id_1 = m_level_background->get_tile_id_at_position(morpheus::core::gfx::Vector2(
                                                                         player_position.get_x(),
                                                                         player_position.get_y() +
                                                                         PLAYER_SIZE.get_y()));
    unsigned int tile_id_2 = m_level_background->get_tile_id_at_position(player_position + PLAYER_SIZE);

    if(collision_tile_id(tile_id_1) || collision_tile_id(tile_id_2)) {
        m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), 0);
    } else {
        m_velocity = morpheus::core::gfx::Vector2(m_velocity.get_x(), m_velocity.get_y() + GRAVITY);
    }
}
