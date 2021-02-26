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
        Jewel();

        virtual ~Jewel() {}

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

        JewelCollision update_jewel(Jewel *jewel, const JewelSide jewel_side) {
            JewelCollision base_jewel_collision;

            switch(jewel_side) {
                case JewelSide::Up:
                    m_north_jewel = jewel;
                    break;
                case JewelSide::Down:
                    m_south_jewel = jewel;
                    break;
                case JewelSide::Left:
                    m_west_jewel = jewel;
                    break;
                case JewelSide::Right:
                    m_east_jewel = jewel;
                    break;
            }

            base_jewel_collision.direction = jewel_side;
            base_jewel_collision.type = m_jewel_type;

            check_collision(base_jewel_collision);

            return base_jewel_collision;
        }

        void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority) override {
            if(m_jewel_sprite != nullptr) {
                m_jewel_sprite->draw(obj_attr_buffer, obj_attr_num, priority);
            }
        }

        void toggle_light_palette();
    protected:
        void input(morpheus::core::InputEvent input_event) override {}
        void update(unsigned char cycle_time) override {}
    private:
        // in this order: circle spawned?, diamond spawned?, square spawned?, triangle spawned?
        static bool jewel_types_spawned[4];

        #ifdef _NDS
            // in same order as jewel_types_spawned
            static unsigned short *jewel_oam_pointers[4];
        #endif

        JewelCollision check_collision(JewelCollision &base_jewel_collision);

        Jewel *m_north_jewel;
        Jewel *m_west_jewel;
        Jewel *m_east_jewel;
        Jewel *m_south_jewel;

        std::unique_ptr<morpheus::core::Node> m_jewel_sprite;
        puzzler::JewelType m_jewel_type;
        unsigned int m_palette_id = 0;
        morpheus::core::gfx::Vector2 m_pre_position;
        unsigned short m_tile_id;
        bool m_using_light_palette = false;
    };
}

#endif //PUZZLER_JEWEL_HPP
