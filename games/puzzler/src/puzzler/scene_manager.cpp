//
// Created by bobby on 09/03/2021.
//

#include "scene_manager.hpp"

puzzler::SceneManager::SceneManager(morpheus::core::MainLoop *main_loop) {
    m_main_loop = main_loop;

    set_drawn_node(false);//

    m_current_scene.reset(new puzzler::MainMenuScene(m_main_loop));

    add_child(m_current_scene.get());

    m_current_scene->setup();
}

void puzzler::SceneManager::update(unsigned char cycle_time) {
    if(m_current_scene->is_marked_for_deletion()) {
        remove_child(m_current_scene.get());

        switch(m_current_scene_type) {
            case SceneType::MAIN_MENU:
                m_current_scene.reset(new puzzler::MainGameScene(m_main_loop,
                                                                 static_cast<puzzler::MainMenuScene*>(
                                                                        m_current_scene.get())->get_cursor_position()));

                m_current_scene_type = SceneType::GAME;
                break;
            case SceneType::GAME:
                m_current_scene.reset(new puzzler::MainMenuScene(m_main_loop));
                m_current_scene_type = SceneType::MAIN_MENU;
                break;
        }

        add_child(m_current_scene.get());

        m_current_scene->setup();
    }
}
