//
// Created by bobby on 07/02/2021.
//

#include "main_game_scene.hpp"

puzzler::MainGameScene::MainGameScene(std::shared_ptr<morpheus::core::MainLoop> &main_loop) {
    m_main_loop = main_loop;

    #ifdef _GBA
        m_user_background.reset(new morpheus::gba::gfx::TiledBackground(
                                                0, std::static_pointer_cast<morpheus::gba::GbaMainLoop>(m_main_loop),
                                                false, 1, 1));
    #elif _NDS
        m_user_background.reset(new morpheus::nds::gfx::TiledBackground4Bpp(
                                            false, 0, std::static_pointer_cast<morpheus::nds::NdsMainLoop>(m_main_loop),
                                            1, 1));
    #endif
}

void puzzler::MainGameScene::input(morpheus::core::InputEvent input_event) {
    if(input_event.state == morpheus::core::InputState::DOWN) {
        if(m_active_jewel != nullptr) {
            morpheus::core::gfx::Vector2 position = m_active_jewel->get_position();

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADLEFT:
                    position = morpheus::core::gfx::Vector2(std::max(16, position.get_x() - 16), position.get_y());

                    if(is_gem_at_position(position)) {
                        return;
                    }

                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    position = morpheus::core::gfx::Vector2(std::min(144, position.get_x() + 16), position.get_y());

                    if(is_gem_at_position(position)) {
                        return;
                    }

                    break;
            }

            m_active_jewel->set_position(position);
        }
    }
}

void puzzler::MainGameScene::setup() {
    #ifdef _GBA
        morpheus::gba::gfx::Sprite4Bpp sprite;

        sprite.load_into_palette(reinterpret_cast<const unsigned short *>(&circlejewelPal[0]), 0, circlejewelPalLen);
    #elif _NDS
        morpheus::nds::gfx::Sprite8Bpp sprite(false, false);

        sprite.load_into_palette(circlejewelPal, 0, circlejewelPalLen);
    #endif

    m_user_background->load_from_array(maingamescreenTiles, maingamescreenTilesLen, maingamescreenPal,
                                       maingamescreenPalLen, maingamescreenMap, maingamescreenMapLen,
                                       morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
}

void puzzler::MainGameScene::update(unsigned char cycle_time) {
    //std::cout << " time = " << static_cast<unsigned int>(cycle_time) << "\n";

    if(m_active_jewel == nullptr) {
        if(m_current_action_cycle_waiting && m_current_action_cycle == cycle_time) {
            m_current_action_cycle_waiting = false;

            #ifdef _GBA
                m_active_jewel.reset(new Jewel());
            #elif _NDS
                m_active_jewel.reset(new Jewel());
            #endif

            m_active_jewel->set_position(morpheus::core::gfx::Vector2(64, 16));

            add_child(m_active_jewel.get());
            m_jewels.push_back(m_active_jewel);

            m_current_action_cycle_waiting = false;

            //std::cout << "\x1b[6;0H Added jewel\n";
        } else if(!m_current_action_cycle_waiting) {
            //std::cout << "\x1b[5;0H Cycle time set to " << static_cast<unsigned int>(cycle_time) << "\n";

            m_current_action_cycle = cycle_time;
            m_current_action_cycle_waiting = true;
        }
    } else {
        if(m_current_action_cycle_waiting && m_current_action_cycle == cycle_time) {
            bool collides_with_jewel;
            morpheus::core::gfx::Vector2 position = m_active_jewel->get_position();

            position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() + 16);

            collides_with_jewel = is_gem_at_position(position);

            if(position.get_y() > 144 || collides_with_jewel) {
                if(collides_with_jewel) {
                    // Graph updating code
                }

                m_active_jewel = nullptr;
                //std::cout << "\x1b[9;0H Jewel finished completely\n";
            } else {
                m_active_jewel->set_position(position);
            }

            ++m_cycles;

            //std::cout << "\x1b[8;0H Jewel finished: " << m_cycles << "\n";
        } else if(!m_current_action_cycle_waiting) {
            //std::cout << "\x1b[7;0H Jewel started\n";

            m_current_action_cycle = cycle_time;
            m_current_action_cycle_waiting = true;
        }
    }
}

bool puzzler::MainGameScene::is_gem_at_position(morpheus::core::gfx::Vector2 position) {
    for(std::shared_ptr<Jewel> &jewel : m_jewels) {
        if(jewel != m_active_jewel) {
            if(jewel->get_position() == position) {
                return true;
            }
        }
    }

    return false;
}

