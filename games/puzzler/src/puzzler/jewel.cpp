//
// Created by bobby on 10/02/2021.
//

#include "jewel.hpp"

bool puzzler::Jewel::jewel_types_spawned[4] = {false, false, false, false};

#ifdef _NDS
unsigned short *puzzler::Jewel::jewel_oam_pointers[4] = {nullptr, nullptr, nullptr, nullptr};
#endif

puzzler::Jewel::Jewel() {
    const unsigned short *tile_array = nullptr;
    int random_number = morpheus::core::MainLoop::get_random_number(0, 4);

    std::cout << "random num = " << random_number << "\n";

    switch(random_number) {
        case 0:
            m_jewel_type = JewelType::Circle;
            m_palette_id = 8;
            m_tile_id = 1;

            tile_array = reinterpret_cast<const unsigned short*>(&circlejewelTiles[0]);

            break;
        case 1:
            m_jewel_type = JewelType::Diamond;
            m_palette_id = 6;
            m_tile_id = 2;

            tile_array = reinterpret_cast<const unsigned short*>(&diamondjewelTiles[0]);

            break;
        case 2:
            m_jewel_type = JewelType::Square;
            m_palette_id = 0;
            m_tile_id = 3;

            tile_array = reinterpret_cast<const unsigned short*>(&squarejewelTiles[0]);

            break;
        case 3:
            m_jewel_type = JewelType::Triangle;
            m_palette_id = 2;
            m_tile_id = 4;

            tile_array = reinterpret_cast<const unsigned short*>(&trianglejewelTiles[0]);

            break;
    }

    if(jewel_types_spawned[random_number]) {
        #ifdef _GBA
            m_jewel_sprite.reset(new morpheus::gba::gfx::Sprite4Bpp(m_tile_id, m_palette_id, 16, 16));


            reinterpret_cast<morpheus::gba::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->set_position(m_pre_position);
        #elif _NDS
            m_jewel_sprite.reset(new morpheus::nds::gfx::Sprite4Bpp(false,
                                                                       jewel_oam_pointers[random_number],
                                                                       16, 16));

            reinterpret_cast<morpheus::nds::gfx::Sprite4Bpp*>(m_jewel_sprite.get())->set_position(m_pre_position);
        #endif
    } else {
        #ifdef _GBA
            auto *sprite4Bpp = new morpheus::gba::gfx::Sprite4Bpp();

            m_jewel_sprite.reset(sprite4Bpp);

            sprite4Bpp->set_position(m_pre_position);

            sprite4Bpp->load_from_array(tile_array, m_palette_id,16, 16, m_tile_id);
        #elif _NDS
            auto *sprite_4_bpp = new morpheus::nds::gfx::Sprite4Bpp(false);

            m_jewel_sprite.reset(sprite_4_bpp);

            std::cout << "loading 4bpp sprite\n";

            sprite_4_bpp->set_position(m_pre_position);

            assert(sprite_4_bpp->load_from_array(tile_array, m_palette_id, 16, 16));
            jewel_oam_pointers[random_number] = sprite_4_bpp->get_gfx_pointer();
        #endif

        jewel_types_spawned[random_number] = true;
    }
}

puzzler::JewelCollision puzzler::Jewel::check_collision(puzzler::JewelCollision &base_jewel_collision) {
    if(base_jewel_collision.collisions.empty() || base_jewel_collision.type == m_jewel_type) {
        base_jewel_collision.collisions.push_back(this);

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
        }//
    }

    return base_jewel_collision;
}
