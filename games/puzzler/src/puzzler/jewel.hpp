//
// Created by bobby on 10/02/2021.
//

#ifndef PUZZLER_JEWEL_HPP
#define PUZZLER_JEWEL_HPP

#include <memory>

#ifdef _GBA
#include <morpheus/gba/gba.hpp>
#elif _NDS
#include <morpheus/nds/nds.hpp>
#endif

#include "action_timer.hpp"

// Grit OBJ includes
#include "circlejewel.h"
#include "diamondjewel.h"
#include "squarejewel.h"
#include "trianglejewel.h"

namespace puzzler {
    enum class JewelType {
        Circle,
        Diamond,
        Square,
        Triangle
    };

    enum class JewelSide {
        Up,
        Down,
        Left,
        Right
    };

    class Jewel;

    struct JewelCollision {
        std::vector<Jewel*> collisions;
        JewelSide direction;
        JewelType type;
    };

    class Jewel : public morpheus::core::ControlReciever {
    public:
        Jewel(morpheus::core::MainLoop *main_loop, int jewel_ground, bool new_scene_reset = false);

        ~Jewel() override;

        puzzler::JewelType get_jewel_type() const {
            return m_jewel_type;
        }

        JewelCollision update_jewel(Jewel *jewel, const JewelSide jewel_side);

        void disconnect_jewel();

        std::string to_string();

        morpheus::core::gfx::Vector2 get_position() const {
            if(m_jewel_sprite == nullptr) {
                return m_pre_position;
            } else {
                return m_jewel_sprite->get_position();
            }
        }

        void set_position(const morpheus::core::gfx::Vector2 position) {
            if(m_jewel_sprite == nullptr) {
                m_pre_position = position;
            } else {
                m_jewel_sprite->set_position(position);
            }
        }

        void toggle_light_palette();
        void transition_deactive() {
            m_active = false;
            m_node_drawn = false;
        }
    protected:
        void input(morpheus::core::InputEvent input_event) override {}
        void update(unsigned char cycle_time)override;
    private:
        // in this order: circle spawned?, diamond spawned?, square spawned?, triangle spawned?
        static bool jewel_types_spawned[4];

        #ifdef _NDS
            // in same order as jewel_types_spawned
            static unsigned short *jewel_oam_pointers[4];
        #endif

        JewelCollision check_collision(JewelCollision &base_jewel_collision);

        Jewel *m_north_jewel = nullptr;
        Jewel *m_west_jewel = nullptr;
        Jewel *m_east_jewel = nullptr;
        Jewel *m_south_jewel = nullptr;

        bool m_active = true;
        bool m_node_drawn = false;
        ActionTimer m_gravity_timer;
        morpheus::core::MainLoop *m_main_loop;
        int m_jewel_ground;
        std::shared_ptr<morpheus::core::gfx::SpriteBase> m_jewel_sprite;
        puzzler::JewelType m_jewel_type;
        unsigned int m_palette_id = 0;
        morpheus::core::gfx::Vector2 m_past_position;
        morpheus::core::gfx::Vector2 m_pre_position;
        unsigned short m_tile_id;
        bool m_using_light_palette = false;
    };
}

#endif //PUZZLER_JEWEL_HPP
