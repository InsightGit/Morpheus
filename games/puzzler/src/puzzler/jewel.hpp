//
// Created by bobby on 10/02/2021.
//

#ifndef PUZZLER_JEWEL_HPP
#define PUZZLER_JEWEL_HPP

#include <memory>

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
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

    class Jewel : public morpheus::core::Node {
    public:
        Jewel(morpheus::core::MainLoop *main_loop, int jewel_ground, bool new_scene_reset = false);

        ~Jewel() override = default;

        puzzler::JewelType get_jewel_type() const {
            return m_jewel_type;
        }

        morpheus::core::gfx::Vector2 get_position() const {
            if(m_jewel_sprite == nullptr) {
                return m_pre_position;
            } else {
                #ifdef _GBA
                    return reinterpret_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->get_position();
                #elif _NDS
                    return reinterpret_cast<morpheus::nds::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->get_position();
                #endif
            }
        }

        void set_position(const morpheus::core::gfx::Vector2 position) {
            if(m_jewel_sprite == nullptr) {
                m_pre_position = position;
            } else {
                #ifdef _GBA
                    reinterpret_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->set_position(position);
                #elif _NDS
                    reinterpret_cast<morpheus::nds::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->set_position(position);
                #endif
            }
        }

        JewelCollision update_jewel(Jewel *jewel, const JewelSide jewel_side);

        void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) override {
            if(m_jewel_sprite != nullptr) {
                m_jewel_sprite->draw(obj_attr_buffer, obj_attr_num, priority);

                if(!m_node_drawn) {
                    /*if(m_active) {
                        m_main_loop->send_to_debug_window(to_string() + " drawn while active");
                    } else {
                        m_main_loop->send_to_debug_window(to_string() + " drawn while inactive");
                    }*/

                    m_node_drawn = true;
                }
            }
        }

        void disconnect_jewel();

        std::string to_string();

        void toggle_light_palette();
        void transition_deactive() {
            m_active = false;
            m_node_drawn = false;
        }
    protected:
        void on_visible_state_changed(bool new_visible_state) override {
            if(m_jewel_sprite != nullptr) {
                if(new_visible_state) {
                    m_jewel_sprite->show();
                } else {
                    m_jewel_sprite->hide();
                }
            }
        }

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
        std::unique_ptr<morpheus::core::Node> m_jewel_sprite;
        puzzler::JewelType m_jewel_type;
        unsigned int m_palette_id = 0;
        morpheus::core::gfx::Vector2 m_past_position;
        morpheus::core::gfx::Vector2 m_pre_position;
        unsigned short m_tile_id;
        bool m_using_light_palette = false;
    };
}

#endif //PUZZLER_JEWEL_HPP
