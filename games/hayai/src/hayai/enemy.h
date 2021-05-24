//
// Created by bobbyy on 5/23/21.
//

#ifndef HAYAI_ENEMY_H
#define HAYAI_ENEMY_H

#ifdef _GBA
    #include <gba/gba.hpp>
#elif _NDS
    #include <nds/nds.hpp>
#endif

namespace hayai {
    class Enemy : public morpheus::core::ControlReciever {
    public:
        Enemy(morpheus::core::gfx::BlendingController *blending_controller,
              morpheus::core::gfx::MosaicController *mosaic_controller);

        ~Enemy() override = default;

        void input(const morpheus::core::InputEvent input_event) override {}

        void update(const unsigned char cycle_time)override;
    private:
        std::unique_ptr<morpheus::core::gfx::SpriteBase> m_enemy_sprite;
    };
}

#endif //HAYAI_ENEMY_H
