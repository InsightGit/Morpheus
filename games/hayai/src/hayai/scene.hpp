//
// Created by bobby on 04/05/2021.
//

#ifndef HAYAI_SCENE_HPP
#define HAYAI_SCENE_HPP

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#else
#error Unsupported platform!
#endif

namespace hayai {
    class Scene : public morpheus::core::ControlReciever {
    public:
        Scene(std::shared_ptr<morpheus::core::MainLoop> main_loop) {
            m_main_loop = main_loop;
        }

        ~Scene() override = default;

        virtual void draw() = 0;
    protected:
        std::shared_ptr<morpheus::core::MainLoop> get_main_loop() const {
            return m_main_loop;
        }
    private:
        std::shared_ptr<morpheus::core::MainLoop> m_main_loop;
    };
}

#endif //HAYAI_SCENE_HPP
