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

// Background includes
#include "mainmenuscreen.h"
#ifdef _NDS
#include "submainmenuscreen.h"
#endif

// Sprite includes
#include "menucursor.h"

#include "scene.hpp"

namespace puzzler {
    class MainMenuScene : public puzzler::Scene {
    public:
        MainMenuScene(morpheus::core::MainLoop *main_loop);

        ~MainMenuScene() override = default;

        void setup()override;
    protected:
        void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) override {
            if(m_cursor != nullptr) {
                m_cursor->draw(obj_attr_buffer, obj_attr_num, priority);
            }
        }

        void on_visible_state_changed(bool new_visible_state) override {}

        void input(morpheus::core::InputEvent input_event)override;
        void update(unsigned char cycle_time)override;
    private:
        const int MAIN_BG_CBB_NUM = 1;
        const int MAIN_BG_SBB_NUM = 1;
        // positions for the easy, medium, and hard board sections
        const std::array<morpheus::core::gfx::Vector2, 3> MENU_POSITIONS{morpheus::core::gfx::Vector2(60, 60),
                                                                         morpheus::core::gfx::Vector2(60, 84),
                                                                         morpheus::core::gfx::Vector2(60, 100)};
        const int TITLE_TEXT_CBB_NUM = 2;
        const int TITLE_TEXT_SBB_NUM = 31;


        std::unique_ptr<morpheus::core::Node> m_cursor;
        unsigned int m_cursor_position;
        std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase> m_main_bg;
        morpheus::core::MainLoop *m_main_loop;

        #ifdef _NDS
            std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase> m_sub_bg;
            PrintConsole m_title_console;
        #endif
    };
}

#endif //PUZZLER_MAIN_MENU_SCENE_HPP
