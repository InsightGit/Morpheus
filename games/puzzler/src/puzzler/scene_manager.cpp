//
// Created by bobby on 09/03/2021.
//

#include "scene_manager.hpp"

puzzler::SceneManager::SceneManager(morpheus::core::MainLoop *main_loop) {
    m_main_loop = main_loop;

    m_current_scene.reset(new puzzler::MainMenuScene(m_main_loop));

    m_main_loop->add_control_reciever(m_current_scene);

    //nocashMessage("setting up");

    m_current_scene->setup();

    //nocashMessage("set up!");
}

void puzzler::SceneManager::update(unsigned char cycle_time) {
    if(m_current_scene->is_marked_for_deletion()) {
        m_main_loop->remove_control_reciever(m_current_scene);

        switch(m_current_scene_type) {
            case SceneType::MAIN_MENU:
                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("resetting scene");

                m_current_scene.reset(new puzzler::MainGameScene(m_main_loop,
                                                                 static_cast<puzzler::MainMenuScene*>(
                                                                        m_current_scene.get())->get_cursor_position()));

                m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("reset scene");

                m_current_scene_type = SceneType::GAME;
                break;
            case SceneType::GAME:
                m_current_scene.reset(new puzzler::MainMenuScene(m_main_loop));
                m_current_scene_type = SceneType::MAIN_MENU;
                break;
        }

        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("adding control reciever");

        m_main_loop->add_control_reciever(m_current_scene);

        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("setting up");
        m_current_scene->setup();

        m_main_loop->get_no_cash_debug_controller()->send_to_debug_window("setup");
    }
}
