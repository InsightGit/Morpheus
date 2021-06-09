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
    class SceneManager : public morpheus::core::ControlReciever {
    public:
        explicit SceneManager(morpheus::core::MainLoop *main_loop);

        virtual ~SceneManager() = default;
    protected:
        void input(morpheus::core::InputEvent input_event) override {}

        virtual void update(unsigned char cycle_time)override;
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
