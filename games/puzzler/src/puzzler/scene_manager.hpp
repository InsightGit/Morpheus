//
// Created by bobby on 09/03/2021.
//

#ifndef PUZZLER_SCENE_MANAGER_HPP
#define PUZZLER_SCENE_MANAGER_HPP

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "main_game_scene.hpp"
#include "main_menu_scene.hpp"
#include "scene.hpp"

namespace puzzler {
    class SceneManager : public morpheus::core::Node {
    public:
        explicit SceneManager(morpheus::core::MainLoop *main_loop);
    protected:
        void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) {
            if(m_current_scene != nullptr) {
                m_current_scene->draw(obj_attr_buffer, obj_attr_num, priority);
            }
        }

        void input(morpheus::core::InputEvent input_event) {
            if(m_current_scene != nullptr) {
                m_current_scene->received_input(input_event);
            }
        }

        void on_visible_state_changed(bool new_visible_state) {
            // not needed because SceneManager should be the root and always visible
        }

        virtual void update(unsigned char cycle_time);
    private:
        enum class SceneType {
            MAIN_MENU,
            GAME
        };

        std::unique_ptr<Scene> m_current_scene;
        SceneType m_current_scene_type = SceneType::MAIN_MENU;
        morpheus::core::MainLoop *m_main_loop;
    };
}

#endif //PUZZLER_SCENE_MANAGER_HPP
