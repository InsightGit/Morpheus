//
// Created by bobby on 06/03/2021.
//

#ifndef PUZZLER_MAIN_MENU_SCENE_HPP
#define PUZZLER_MAIN_MENU_SCENE_HPP

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "scene.hpp"

namespace puzzler {
    class MainMenuScene : public puzzler::Scene {
    public:
        MainMenuScene();

        void setup()override;
    protected:
        void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) override {
            if(m_active_jewel != nullptr) {
                m_active_jewel->draw(obj_attr_buffer, obj_attr_num, priority);
            }
        }

        void on_visible_state_changed(bool new_visible_state) override {}

        void input(morpheus::core::InputEvent input_event)override;
        void update(unsigned char cycle_time)override;
    private:
        std::unique_ptr<morpheus::core::Node> m_cursor;
    };
}

#endif //PUZZLER_MAIN_MENU_SCENE_HPP
