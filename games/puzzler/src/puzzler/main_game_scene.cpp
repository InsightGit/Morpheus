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
            std::vector<morpheus::core::gfx::Vector2> positions;

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADLEFT:
                    positions.emplace_back(std::max(16, position.get_x() + 16),
                                           position.get_y());

                    position = morpheus::core::gfx::Vector2(std::max(16, position.get_x() - 16), position.get_y());

                    positions.push_back(position);

                    if(is_gem_at_positions(positions) || position.get_y() >= 144) {
                        return;
                    }

                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    positions.emplace_back(position.get_x(),position.get_y() + 16);

                    position = morpheus::core::gfx::Vector2(std::min(144, position.get_x() + 16), position.get_y());

                    positions.push_back(position);

                    if(is_gem_at_positions(positions) || position.get_y() >= 144) {
                        return;
                    }

                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    position = morpheus::core::gfx::Vector2(morpheus::core::gfx::Vector2(position.get_x(),
                                                                                         position.get_y() + 16));

                    if(is_gem_at_position(position) || position.get_y() >= 144) {
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
    #ifdef _NDS
        // intentional copy
        unsigned short old_palette_value = BG_PALETTE[0];

        m_score_console = *consoleGetDefault();

        std::cout << "Printing score\n";

        consoleInit(&m_score_console, 1, BgType_Text4bpp, BgSize_T_256x256, SCORE_TEXT_MAP_BASE,
                    SCORE_TEXT_TILE_BASE, true, true);

        std::cout << "\x1b[3;20H \x1b[40;5m Score:0";

        BG_PALETTE[0] = old_palette_value;

        morpheus::nds::NdsMainLoop::reset_to_debug_print_console();

        std::cout << "Printed score\n";
    #endif
}

void puzzler::MainGameScene::update(unsigned char cycle_time) {
    //std::cout << " time = " << static_cast<unsigned int>(cycle_time) << "\n";

    bgSetPriority(0, 1);

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
            std::vector<Jewel*> jewel_collisions;
            std::vector<JewelCollision> jewel_collision_results;
            morpheus::core::gfx::Vector2 position = m_active_jewel->get_position();
            std::vector<morpheus::core::gfx::Vector2> positions;

            position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() + 16);

            positions.push_back(position);

            jewel_collisions = get_gems_at_positions(positions);

            if(position.get_y() > 144 || !jewel_collisions.empty()) {
                if(!jewel_collisions.empty()) {
                    JewelCollision jewel_collision_result;

                    assert(jewel_collisions.size() == 1);

                    jewel_collision_result = m_active_jewel->update_jewel(jewel_collisions[0],
                                                                          JewelSide::Up);

                    if(!jewel_collision_result.collisions.empty()) {
                        jewel_collision_results.push_back(jewel_collision_result);
                    }
                }

                positions.clear();

                positions.emplace_back(position.get_x() - 16, position.get_y());
                positions.emplace_back(position.get_x() + 16, position.get_y());

                jewel_collisions = get_gems_at_positions(positions);

                for(Jewel *jewel : jewel_collisions) {
                    JewelCollision jewel_collision_result;

                    if(jewel->get_position().get_x() == position.get_x() - 16) {
                        jewel_collision_result = m_active_jewel->update_jewel(jewel, JewelSide::Left);
                    } else if(jewel->get_position().get_x() == position.get_x() + 16) {
                        jewel_collision_result = m_active_jewel->update_jewel(jewel, JewelSide::Right);
                    }

                    std::cout << "collision size: " << jewel_collision_result.collisions.size() << "\n";

                    if(jewel_collision_result.collisions.size() > 1) {
                        jewel_collision_results.push_back(jewel_collision_result);
                    }
                }

                //std::cout << "\x1b[9;0H Jewel completely finished\n";

                update_gem_scoring(jewel_collision_results);

                m_active_jewel = nullptr;
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

void puzzler::MainGameScene::update_gem_scoring(std::vector<JewelCollision> jewel_collision_results) {
    for(JewelCollision &jewel_collision_result : jewel_collision_results) {
        switch(jewel_collision_result.collisions.size()) {
            case 3:
                m_total_score += 10;
                break;
            case 4:
                m_total_score += 25;
                break;
            case 5:
                m_total_score += 50;
                break;
            case 6:
                m_total_score += 100;
                break;
            default:
                continue;
        }

        for(unsigned int i = 0; m_jewels.size() > i; ++i) {
            std::cout << "Destroying jewels\n";

            for(unsigned int i2 = 0; jewel_collision_result.collisions.size() > i2; ++i2) {
                // TODO(Bobby): better equality check
                if(m_jewels[i]->get_position() == jewel_collision_result.collisions[i2]->get_position()) {
                    jewel_collision_result.collisions.erase(jewel_collision_result.collisions.begin() + i2);

                    // in case shared_ptr messes up
                    // delete jewel_collision_result.collisions[i2];
                    std::cout << "Jewels destroyed\n";
                }
            }
        }
    }
}

std::vector<puzzler::Jewel*> puzzler::MainGameScene::get_gems_at_positions(
        std::vector<morpheus::core::gfx::Vector2> positions) {
    std::vector<puzzler::Jewel*> return_value;

    for(std::shared_ptr<Jewel> &jewel : m_jewels) {
        if(jewel != m_active_jewel) {
            for(unsigned int i = 0; positions.size() > i; ++i) {
                if(jewel->get_position() == positions[i]) {
                    return_value.push_back(jewel.get());

                    positions.erase(positions.begin() + i);
                }
            }
        }
    }

    return return_value;
}

bool puzzler::MainGameScene::is_gem_at_positions(std::vector<morpheus::core::gfx::Vector2> positions) {
    for(std::shared_ptr<Jewel> &jewel : m_jewels) {
        if(jewel != m_active_jewel) {
            for(morpheus::core::gfx::Vector2 &position : positions) {
                if(jewel->get_position() == position) {
                    return true;
                }
            }
        }
    }
    return false;
}
