//
// Created by bobby on 10/02/2021.
//

#include "jewel.hpp"

#include "main_game_scene.hpp"

bool puzzler::Jewel::jewel_types_spawned[4] = {false, false, false, false};

#ifdef _NDS
unsigned short *puzzler::Jewel::jewel_oam_pointers[4] = {nullptr, nullptr, nullptr, nullptr};
#endif

puzzler::Jewel::Jewel(morpheus::core::MainLoop *main_loop) {
    int random_number;
    const unsigned short *tile_array = nullptr;

    m_main_loop = main_loop;
    random_number = m_main_loop->get_random_number(0, 4);

    switch(random_number) {
        case 0:
            m_jewel_type = JewelType::Circle;
            m_palette_id = 8;
            m_tile_id = 0;

            tile_array = reinterpret_cast<const unsigned short*>(&circlejewelTiles[0]);

            break;
        case 1:
            m_jewel_type = JewelType::Diamond;
            m_palette_id = 6;
            m_tile_id = 5;

            tile_array = reinterpret_cast<const unsigned short*>(&diamondjewelTiles[0]);

            break;
        case 2:
            m_jewel_type = JewelType::Square;
            m_palette_id = 0;
            m_tile_id = 10;

            tile_array = reinterpret_cast<const unsigned short*>(&squarejewelTiles[0]);

            break;
        case 3:
            m_jewel_type = JewelType::Triangle;
            m_palette_id = 2;
            m_tile_id = 15;

            tile_array = reinterpret_cast<const unsigned short*>(&trianglejewelTiles[0]);

            break;
    }

    if(jewel_types_spawned[random_number]) {
        #ifdef _GBA
            m_jewel_sprite.reset(new morpheus::gba::gfx::Sprite4Bpp(m_tile_id, m_palette_id, 16, 16));

            reinterpret_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->set_position(m_pre_position);
        #elif _NDS
            m_jewel_sprite.reset(new morpheus::nds::gfx::Sprite4Bpp(false, jewel_oam_pointers[random_number], 16, 16));

            auto *sprite = reinterpret_cast<morpheus::nds::gfx::Sprite4Bpp*>(m_jewel_sprite.get());

            sprite->set_palette_id(m_palette_id);
        #endif
    } else {
        #ifdef _GBA
            auto *sprite_4_bpp = new morpheus::gba::gfx::Sprite4Bpp(m_palette_id);

            static_cast<morpheus::gba::gfx::Sprite*>(sprite_4_bpp)->load_from_array(tile_array, 16, 16,
                                                                                    m_tile_id);

            sprite_4_bpp->set_position(m_pre_position);

            m_jewel_sprite.reset(sprite_4_bpp);
        #elif _NDS
            auto *sprite_4_bpp = new morpheus::nds::gfx::Sprite4Bpp(false);

            m_jewel_sprite.reset(sprite_4_bpp);

            sprite_4_bpp->set_position(m_pre_position);

            sassert(sprite_4_bpp->load_from_array(tile_array, m_palette_id, 16, 16), "sprite not properly loaded");
            jewel_oam_pointers[random_number] = sprite_4_bpp->get_gfx_pointer();
        #endif

        jewel_types_spawned[random_number] = true;
    }
}

puzzler::JewelCollision puzzler::Jewel::check_collision(puzzler::JewelCollision &base_jewel_collision) {
    if(base_jewel_collision.collisions.empty() || base_jewel_collision.type == m_jewel_type) {
        if(get_position() != morpheus::core::gfx::Vector2(0, 0)) {
            base_jewel_collision.collisions.push_back(this);
        }

        switch (base_jewel_collision.direction) {
            case JewelSide::Up:
                if(m_north_jewel != nullptr) {
                    m_north_jewel->check_collision(base_jewel_collision);
                }
                break;
            case JewelSide::Left:
                if(m_west_jewel != nullptr) {
                    m_west_jewel->check_collision(base_jewel_collision);
                }
                break;
            case JewelSide::Right:
                if(m_east_jewel != nullptr) {
                    m_east_jewel->check_collision(base_jewel_collision);
                }
                break;
            case JewelSide::Down:
                if(m_south_jewel != nullptr) {
                    m_south_jewel->check_collision(base_jewel_collision);
                }
                break;
        }
    }

    return base_jewel_collision;
}

void puzzler::Jewel::toggle_light_palette() {
    if(m_using_light_palette) {
        m_palette_id--;
    } else {
        m_palette_id++;
    }

    m_using_light_palette = !m_using_light_palette;

    #ifdef _GBA
        reinterpret_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->set_palette_id(m_palette_id);
    #elif _NDS
        reinterpret_cast<morpheus::nds::gfx::Sprite*>(m_jewel_sprite.get())->set_palette_id(m_palette_id);
    #endif
}

void puzzler::Jewel::update(unsigned char cycle_time) {
    morpheus::core::gfx::Vector2 current_position = get_position();

    if(m_south_jewel == nullptr && current_position.get_y() < puzzler::JEWEL_GROUND && !m_active) {
        if(m_gravity_timer.current_action_cycle_waiting && m_gravity_timer.current_action_cycle == cycle_time) {
            set_position(morpheus::core::gfx::Vector2(current_position.get_x(),
                                                           current_position.get_y() + 16));

            // insures no gravity desync if this jewel has already been updated before this Jewel's update function
            m_north_jewel->update(cycle_time);
        } else if(!m_gravity_timer.current_action_cycle_waiting) {
            m_gravity_timer.current_action_cycle = cycle_time;
            m_gravity_timer.current_action_cycle_waiting = true;
        }
    } else {
        if(m_south_jewel != nullptr && !m_active && current_position.get_y() < puzzler::JEWEL_GROUND) {
            set_position(morpheus::core::gfx::Vector2(current_position.get_x(),
                                                           m_south_jewel->get_position().get_y() - 16));
        }

        m_gravity_timer.current_action_cycle_waiting = false;
    }
}

void puzzler::Jewel::disconnect_jewel() {
    m_main_loop->send_to_debug_window("disconnect jewel called");

    if(m_north_jewel != nullptr && m_north_jewel->m_south_jewel == this) {
        m_main_loop->send_to_debug_window("south jewel nullified");
        m_north_jewel->m_south_jewel = nullptr;
    }

    if(m_west_jewel != nullptr && m_west_jewel->m_east_jewel == this) {
        m_west_jewel->m_east_jewel = nullptr;
    }

    if(m_east_jewel != nullptr && m_east_jewel->m_west_jewel == this) {
        m_east_jewel->m_west_jewel = nullptr;
    }

    if(m_south_jewel != nullptr && m_south_jewel->m_north_jewel == this) {
        m_south_jewel->m_north_jewel = nullptr;
    }
}

std::string puzzler::Jewel::to_string() {
    std::string jewel_type_string;

    switch(m_jewel_type) {
        case JewelType::Circle:
            jewel_type_string = "Circle";
            break;
        case JewelType::Diamond:
            jewel_type_string = "Diamond";
            break;
        case JewelType::Square:
            jewel_type_string = "Square";
            break;
        case JewelType::Triangle:
            jewel_type_string = "Triangle";
            break;
    }

    return jewel_type_string + " jewel at " + get_position().to_string();
}
