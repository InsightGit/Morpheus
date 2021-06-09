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

    class Jewel : public morpheus::core::gfx::SpriteBase {
    public:
        Jewel(morpheus::core::MainLoop *main_loop, int jewel_ground, bool new_scene_reset = false);

        ~Jewel() override = default;

        puzzler::JewelType get_jewel_type() const {
            return m_jewel_type;
        }

        morpheus::core::gfx::SpriteSize get_sprite_size() const override {
            return m_jewel_sprite->get_sprite_size();
        }

        bool is_blended() const override {
            return m_jewel_sprite->is_blended();
        }

        bool load_into_palette(const unsigned short *palette, const unsigned int pal_len,
                               const unsigned int pal_offset = 256) override {
            return m_jewel_sprite->load_into_palette(palette, pal_len, pal_offset);
        }

        void set_sprite_size(morpheus::core::gfx::SpriteSize size) override {
            m_jewel_sprite->set_sprite_size(size);
        }
    protected:
        void draw_node(std::vector<void *> &obj_attr_buffer, unsigned int obj_attr_num) override {
            if(m_jewel_sprite != nullptr) {
                m_jewel_sprite->draw(obj_attr_buffer, obj_attr_num);

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

        void on_visible_state_changed(bool new_visible_state) override {
            if(m_jewel_sprite != nullptr) {
                if(new_visible_state) {
                    m_jewel_sprite->show();
                } else {
                    m_jewel_sprite->hide();
                }
            }
        }

        void mosaic_state_updated() override {}
        void toggle_blending(bool enable_blending, bool bottom_layer = true) override {}
        void update_affine_state(morpheus::core::gfx::AffineTransformation affine_transformation,
                                 bool new_transformation) override {}

        void resume_animation() override {}
        void stop_animation(bool pause) override {}

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
        std::unique_ptr<morpheus::core::gfx::SpriteBase> m_jewel_sprite;
        puzzler::JewelType m_jewel_type;
        unsigned int m_palette_id = 0;
        morpheus::core::gfx::Vector2 m_past_position;
        morpheus::core::gfx::Vector2 m_pre_position;
        unsigned short m_tile_id;
        bool m_using_light_palette = false;
    };
}

#endif //PUZZLER_JEWEL_HPP
