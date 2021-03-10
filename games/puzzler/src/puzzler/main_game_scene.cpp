//
// Created by bobby on 07/02/2021.
//

#include "main_game_scene.hpp"

puzzler::MainGameScene::MainGameScene(morpheus::core::MainLoop *main_loop, const unsigned int difficulty_setting) {
    m_difficulty_setting = difficulty_setting;
    m_main_loop = main_loop;

    #ifdef _GBA
        m_user_background.reset(new morpheus::gba::gfx::TiledBackground(
                                                0, static_cast<morpheus::gba::GbaMainLoop*>(m_main_loop),
                                                false, 1, 1));
    #elif _NDS
        m_user_background.reset(new morpheus::nds::gfx::TiledBackground4Bpp(
                                            false, 0, reinterpret_cast<morpheus::nds::NdsMainLoop*>(m_main_loop),
                                            1, 1));
    #endif

    // we can load the soundbank in the module and have all the other maxmod stuff just use that
    m_active_module.reset(morpheus::utils::construct_appropriate_max_mod_music(MOD_MAIN_THEME3,
                                                              static_cast<void*>(const_cast<uint8_t*>(soundbank_bin)),
                                                              4));

    // TODO(Bobby): Do we really need a const_cast for this?
    m_jewel_complete_sfx.reset(
            morpheus::utils::construct_appropriate_max_mod_sfx(SFX_GEMCOMPLETED));
    m_jewel_put_sfx.reset(morpheus::utils::construct_appropriate_max_mod_sfx(SFX_GEMPLACED));
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

                    if(is_gem_at_positions(positions) || position.get_y() >= JEWEL_GROUND) {
                        return;
                    }

                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    positions.emplace_back(position.get_x(),position.get_y() + 16);

                    position = morpheus::core::gfx::Vector2(std::min(144, position.get_x() + 16), position.get_y());

                    positions.push_back(position);

                    if(is_gem_at_positions(positions) || position.get_y() >= JEWEL_GROUND) {
                        return;
                    }

                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    position = morpheus::core::gfx::Vector2(morpheus::core::gfx::Vector2(position.get_x(),
                                                                                         position.get_y() + 16));

                    if(is_gem_at_position(position) || position.get_y() >= JEWEL_GROUND) {
                        return;
                    }
                    break;
            }

            m_active_jewel->set_position(position);
        } else if(m_game_over) {
            #ifdef _GBA
                if(input_event.button == morpheus::core::InputButton::START) {
                    m_total_score = 0;
                    m_game_over = false;

                    for(std::unique_ptr<Jewel> &jewel : m_jewels) {
                        if(jewel != nullptr) {
                            remove_child(jewel.get());
                        }
                    }

                    m_jewels.clear();

                    tte_set_pos(192, 16);

                    tte_write("Score:");

                    tte_set_pos(192, 24);

                    tte_write("0                   ");

                    tte_set_pos(80, 72);

                    tte_write("                                      ");

                    tte_set_pos(0, 80);

                    tte_write("                                      ");
                }
            #elif _NDS
                if(input_event.button == morpheus::core::InputButton::TOUCH) {
                    nocashMessage("touch detected");

                    if(m_game_over) {
                        m_main_loop->send_to_debug_window("touch game over triggered. removing jewel children");

                        // time to restart game
                        for(std::unique_ptr<Jewel> &jewel : m_jewels) {
                            if(jewel != nullptr) {
                                remove_child(jewel.get());
                            }
                        }

                        m_main_loop->send_to_debug_window("clearing jewels");

                        m_jewels.clear();

                        m_main_loop->send_to_debug_window("jewels cleared");

                        m_total_score = 0;
                        m_game_over = false;

                        consoleInit(&m_sub_console, 0, BgType_Text4bpp, BgSize_T_256x256,
                                    SCORE_TEXT_MAP_BASE, SCORE_TEXT_TILE_BASE, false, false);

                        consoleClear();

                        consoleInit(&m_score_console, 1, BgType_Text4bpp, BgSize_T_256x256,
                                    SCORE_TEXT_MAP_BASE, SCORE_TEXT_TILE_BASE, true, false);

                        consoleClear();

                        std::cout << "\x1b[3;20H \x1b[40;5m Score:0";

                        morpheus::nds::NdsMainLoop::reset_to_debug_print_console();
                    }
                }
            #endif
        }
    }
}

void puzzler::MainGameScene::setup() {
    m_main_loop->send_to_debug_window("Loading background...");

    switch(m_difficulty_setting) {
        case 0:
            m_user_background->load_from_array(maingamescreenTiles, maingamescreenTilesLen, maingamescreenPal,
                                               maingamescreenPalLen, maingamescreenMap, maingamescreenMapLen,
                                               morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
            break;
        case 1:
            m_user_background->load_from_array(maingamescreenTiles, maingamescreenTilesLen, maingamescreenPal,
                                               maingamescreenPalLen, smallergamescreenMap,
                                               SMALLERGAMESCREEN_TILE_MAP_LENGTH,
                                               morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
            break;
        case 2:
            m_user_background->load_from_array(maingamescreenTiles, maingamescreenTilesLen, maingamescreenPal,
                                               maingamescreenPalLen, smallestgamescreenMap,
                                               SMALLESTGAMESCREEN_TILE_MAP_LENGTH,
                                               morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
            break;
    }

    m_main_loop->send_to_debug_window("Background loaded. Loading gfx...");

    #ifdef _GBA
        morpheus::gba::gfx::Sprite4Bpp sprite;

        sprite.load_into_palette(circlejewelPal, 0, circlejewelPalLen);

        m_main_loop->enable_background(1);

        tte_init_se(1, BG_CBB(2) | BG_SBB(31), 0, CLR_WHITE, 14, nullptr,
                    nullptr);

        tte_set_pos(192, 16);

        tte_write("Score:");

        tte_set_pos(192, 24);

        tte_write("0");
    #elif _NDS
        // intentional copy
        unsigned short old_palette_value = BG_PALETTE[0];

        morpheus::nds::gfx::Sprite8Bpp sprite(false, false);

        sprite.load_into_palette(circlejewelPal, 0, circlejewelPalLen);

        m_score_console = *consoleGetDefault();

        //std::cout << "Printing score\n";

        consoleInit(&m_score_console, 1, BgType_Text4bpp, BgSize_T_256x256, SCORE_TEXT_MAP_BASE,
                    SCORE_TEXT_TILE_BASE, true, true);

        std::cout << "\x1b[3;20H \x1b[40;5m Score:0";

        BG_PALETTE[0] = old_palette_value;

        morpheus::nds::NdsMainLoop::reset_to_debug_print_console();

        //std::cout << "Printed score\n";

        setup_second_screen();
    #endif

    m_main_loop->send_to_debug_window("Loaded gfx. Loading sfx...");

    m_user_background->set_priority(1);

    m_main_loop->send_to_debug_window("Loaded sfx. Loading module...");

    m_active_module->play_music(true);

    m_main_loop->send_to_debug_window("Module loaded.");
}

void puzzler::MainGameScene::update(unsigned char cycle_time) {
    #ifdef _NDS
        bgSetPriority(0, 1);
    #endif

    if(m_game_over) {
        return;
    }

    if(m_active_jewel == nullptr) {
        if(m_jewel_spawning_timer.current_action_cycle_waiting &&
           m_jewel_spawning_timer.current_action_cycle == cycle_time) {
            std::vector<morpheus::core::gfx::Vector2> initial_position_vector = {morpheus::core::gfx::Vector2(64, 16)};

            if(!get_gems_at_positions(initial_position_vector).empty()) {
                // game over!
                #ifdef _GBA
                    tte_set_pos(80, 72);

                    tte_write("Game over!");

                    tte_set_pos(0, 80);

                    tte_write("Press the START button to restart");
                #elif _NDS
                    unsigned short old_palette_value = BG_PALETTE_SUB[0];

                    consoleInit(&m_sub_console, 0, BgType_Text4bpp, BgSize_T_256x256,
                                SCORE_TEXT_MAP_BASE,
                                SCORE_TEXT_TILE_BASE, false, true);

                    BG_PALETTE_SUB[0] = old_palette_value;

                    consoleClear();

                    std::cout << "\x1b[10;8H \x1b[40;5m Game Over!";
                    std::cout << "\x1b[11;2H \x1b[40;5m Tap the screen to restart!";

                    morpheus::nds::NdsMainLoop::reset_to_debug_print_console();
                #endif

                m_game_over = true;

                return;
            }

            m_active_jewel.reset(new Jewel(m_main_loop));

            m_active_jewel->set_position(initial_position_vector[0]);

            add_child(m_active_jewel.get());

            //std::cout << "\x1b[6;0H Added jewel\n";
        } else if(!m_jewel_spawning_timer.current_action_cycle_waiting) {
            //std::cout << "\x1b[5;0H Cycle time set to " << static_cast<unsigned int>(cycle_time) << "\n";

            m_jewel_spawning_timer.current_action_cycle = cycle_time;
            m_jewel_spawning_timer.current_action_cycle_waiting = true;
        }
    } else {
        if(m_jewel_spawning_timer.current_action_cycle_waiting &&
           m_jewel_spawning_timer.current_action_cycle == cycle_time) {
            std::vector<unsigned int> jewel_collisions;
            std::vector<JewelCollision> jewel_collision_results;
            morpheus::core::gfx::Vector2 position = m_active_jewel->get_position();
            std::vector<morpheus::core::gfx::Vector2> positions;

            position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() + 16);

            positions.push_back(position);

            jewel_collisions = get_gems_at_positions(positions);

            if(position.get_y() > JEWEL_GROUND || !jewel_collisions.empty()) {
                if(!jewel_collisions.empty()) {
                    JewelCollision jewel_collision_result;

                    #ifdef _NDS
                        // TODO(Bobby): sassert implementation for the GBA
                        sassert(jewel_collisions.size() == 1, "unexpected jewel collision size");
                    #endif

                    if(m_active_jewel != nullptr) {
                        jewel_collision_result = m_active_jewel->update_jewel(m_jewels[jewel_collisions[0]].get(),
                                                                              JewelSide::Down);
                        m_jewels[jewel_collisions[0]]->update_jewel(m_active_jewel.get(), JewelSide::Up);
                    }

                    if(!jewel_collision_result.collisions.empty()) {
                        jewel_collision_results.push_back(jewel_collision_result);
                    }
                }

                positions.clear();

                positions.emplace_back(position.get_x() - 16, std::min(JEWEL_GROUND, position.get_y() - 16));
                positions.emplace_back(position.get_x() + 16, std::min(JEWEL_GROUND, position.get_y() - 16));

                jewel_collisions = get_gems_at_positions(positions);

                for(unsigned int i : jewel_collisions) {
                    JewelCollision jewel_collision_result;

                    if(m_jewels[i].get()->get_position().get_x() == position.get_x() - 16) {
                        jewel_collision_result = m_active_jewel->update_jewel(m_jewels[i].get(), JewelSide::Left);
                    } else if(m_jewels[i].get()->get_position().get_x() == position.get_x() + 16) {
                        jewel_collision_result = m_active_jewel->update_jewel(m_jewels[i].get(), JewelSide::Right);
                    }

                    //std::cout << "collision size: " << jewel_collision_result.collisions.size() << "\n";

                    if(jewel_collision_result.collisions.size() > 1) {
                        jewel_collision_results.push_back(jewel_collision_result);
                    }
                }

                //std::cout << "\x1b[9;0H Jewel completely finished\n";

                update_gem_scoring(jewel_collision_results);

                if(m_active_jewel != nullptr) {
                    m_active_jewel->transition_deactive();

                    m_jewels.push_back(std::unique_ptr<puzzler::Jewel>());

                    remove_child(m_active_jewel.get());

                    m_jewels[m_jewels.size() - 1] = std::move(m_active_jewel);

                    #ifdef _NDS
                        sassert(m_active_jewel == nullptr, "improperly moved std::unique_ptr");
                    #endif

                    add_child(m_jewels[m_jewels.size() - 1].get());

                    m_jewel_put_sfx->start_effect(true);
                }
            } else {
                m_active_jewel->set_position(position);
            }

            ++m_cycles;

            //std::cout << "\x1b[8;0H Jewel finished: " << m_cycles << "\n";
        } else if(!m_jewel_spawning_timer.current_action_cycle_waiting) {
            //std::cout << "\x1b[7;0H Jewel started\n";

            m_jewel_spawning_timer.current_action_cycle = cycle_time;
            m_jewel_spawning_timer.current_action_cycle_waiting = true;
        }
    }

    if(m_jewel_animation_timer.current_action_cycle_waiting &&
       m_jewel_animation_timer.current_action_cycle == cycle_time) {
        for(unsigned int i = 0; m_jewels.size() > i; ++i) {
            m_jewels[i]->toggle_light_palette();
        }

        if(m_active_jewel != nullptr) {
            m_active_jewel->toggle_light_palette();
        }

        int new_cycle_time = cycle_time + 30;

        if(new_cycle_time > 59) {
            new_cycle_time -= 60;
        }

        m_jewel_animation_timer.current_action_cycle = new_cycle_time;
    } else if(!m_jewel_animation_timer.current_action_cycle_waiting) {
        m_jewel_animation_timer.current_action_cycle = 30;
        m_jewel_animation_timer.current_action_cycle_waiting = true;
    }
}

void puzzler::MainGameScene::update_gem_scoring(std::vector<JewelCollision> jewel_collision_results) {
    for(JewelCollision &jewel_collision_result : jewel_collision_results) {
        if(jewel_collision_result.type == JewelType::Triangle) {
            m_main_loop->send_to_debug_window("Triangle Collision report:");

            for(Jewel *jewel : jewel_collision_result.collisions) {
                if(jewel == nullptr) {
                    m_main_loop->send_to_debug_window("nullptr collision");
                } else {
                    m_main_loop->send_to_debug_window(jewel->to_string());
                }
            }
        }

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

        m_jewels.erase(std::remove_if(m_jewels.begin(), m_jewels.end(),
                                      [this, &jewel_collision_result](std::unique_ptr<Jewel> &jewel) {
            morpheus::core::gfx::Vector2 position = jewel->get_position();

            for(unsigned int i2 = 0; jewel_collision_result.collisions.size() > i2; ++i2) {
                if(jewel_collision_result.collisions[i2] == nullptr) {
                    continue;
                }

                morpheus::core::gfx::Vector2 position2 = jewel_collision_result.collisions[i2]->get_position();

                // TODO(Bobby): better equality check
                if(position == position2) {
                    remove_child(jewel.get());

                    jewel->disconnect_jewel();

                    jewel_collision_result.collisions.erase(jewel_collision_result.collisions.begin() +
                                                            static_cast<int>(i2));

                    return true;
                }
            }

            return false;
        }), m_jewels.end());

        m_active_jewel->disconnect_jewel();

        remove_child(m_active_jewel.get());
        m_active_jewel.reset(nullptr);

        m_jewel_put_sfx->stop_effect();
        m_jewel_complete_sfx->start_effect(false);

        //m_main_loop->send_to_debug_window("effect started");

        #ifdef _GBA
            tte_set_pos(192, 24);

            tte_write("                                   ");

            tte_set_pos(192, 24);

            tte_write(std::to_string(m_total_score).c_str());
        #elif _NDS
            unsigned short old_palette_value = BG_PALETTE[0];

            consoleInit(&m_score_console, 1, BgType_Text4bpp, BgSize_T_256x256, SCORE_TEXT_MAP_BASE,
                        SCORE_TEXT_TILE_BASE, true, false);

            BG_PALETTE[0] = old_palette_value;
            consoleClear();

            std::cout << "\x1b[3;20H \x1b[40;5m Score:" << m_total_score;

            morpheus::nds::NdsMainLoop::reset_to_debug_print_console();
        #endif

        //std::cout << "Score updated\n";
    }
}

std::vector<unsigned int> puzzler::MainGameScene::get_gems_at_positions(
        std::vector<morpheus::core::gfx::Vector2> positions) {
    std::vector<unsigned int> return_value;

    for(unsigned int i = 0; m_jewels.size() > i; ++i) {
        morpheus::core::gfx::Vector2 jewel_position = m_jewels[i]->get_position();

        for(unsigned int i2 = 0; positions.size() > i2; ++i2) {
            if(jewel_position == positions[i2]) {
                return_value.push_back(i);

                positions.erase(positions.begin() + static_cast<int>(i2));
            }
        }
    }

    return return_value;
}

bool puzzler::MainGameScene::is_gem_at_positions(std::vector<morpheus::core::gfx::Vector2> positions) {
    for(std::unique_ptr<Jewel> &jewel : m_jewels) {
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

#ifdef _NDS
    void puzzler::MainGameScene::setup_second_screen() {
        m_sub_background.reset(new morpheus::nds::gfx::TiledBackground4Bpp(
                                            true, 1,
                                            static_cast<morpheus::nds::NdsMainLoop*>(m_main_loop),
                                            1, 1));

        m_sub_background->load_from_array(subscorescreenTiles, subscorescreenTilesLen, subscorescreenPal,
                                          subscorescreenPalLen, subscorescreenMap, subscorescreenMapLen,
                                          morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

        nocashMessage("Second screen loaded.");
    }
#endif