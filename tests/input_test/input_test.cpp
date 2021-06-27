//
// Created by bobby on 26/06/2021.
//

#ifdef _GBA
    #include <gba/gba.hpp>
#elif _NDS
    #include <nds/nds.hpp>
#endif

#include "test8.h"
#include "test4.h"

class MoveableSprite : public morpheus::core::ControlReciever {
public:
    explicit MoveableSprite(morpheus::core::gfx::SpriteBase *sprite_base) {
        m_sprite_base = std::shared_ptr<morpheus::core::gfx::SpriteBase>(sprite_base);
    }

    void add_sprite_base_to_main_loop(std::shared_ptr<morpheus::core::MainLoop> main_loop) {
        main_loop->add_sprite(m_sprite_base);
        main_loop->add_control_reciever(std::shared_ptr<morpheus::core::ControlReciever>(this));
    }

    std::shared_ptr<morpheus::core::gfx::SpriteBase> get_sprite_base() const {
        return m_sprite_base;
    }

    void input(morpheus::core::InputEvent input_event) override {
        morpheus::core::gfx::Vector2 new_position = m_sprite_base->get_position();

        if(input_event.state == morpheus::core::InputState::HELD || \
            input_event.state == morpheus::core::InputState::DOWN) {

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    new_position = morpheus::core::gfx::Vector2(new_position.get_x(), new_position.get_y() - 5);
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    new_position = morpheus::core::gfx::Vector2(new_position.get_x(), new_position.get_y() + 5);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    new_position = morpheus::core::gfx::Vector2(new_position.get_x() - 5, new_position.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    new_position = morpheus::core::gfx::Vector2(new_position.get_x() + 5, new_position.get_y());
                    break;
                default:
                    break;
            }
        }

        m_sprite_base->set_position(new_position);
    }

    void update(unsigned char cycle_time) override {}
private:
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite_base;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    // The nullptrs disable blending and mosaic on these SpriteBases
    MoveableSprite sprite_4bpp(morpheus::utils::construct_appropriate_sprite_4bpp(false, nullptr, nullptr));
    MoveableSprite sprite_4bpp_2(morpheus::utils::construct_appropriate_sprite_4bpp(false, nullptr, nullptr));
    MoveableSprite sprite_8bpp(morpheus::utils::construct_appropriate_sprite_8bpp(false, nullptr, nullptr));
    MoveableSprite sprite_8bpp_2(morpheus::utils::construct_appropriate_sprite_8bpp(false, nullptr, nullptr));

    #ifdef _GBA
        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite_8bpp.get_sprite_base().get())->
            load_from_array(reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                            reinterpret_cast<const unsigned short *>(test8Pal), test8PalLen,
                       morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
        static_cast<morpheus::gba::gfx::Sprite8Bpp*>(sprite_8bpp_2.get_sprite_base().get())->
            load_from_array(reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                            morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);

        static_cast<morpheus::gba::gfx::Sprite4Bpp*>(sprite_4bpp.get_sprite_base().get())->
            load_from_array(reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen,
                            reinterpret_cast<const unsigned short *>(test4Pal), 32,
                            15, morpheus::core::gfx::SpriteSize::SIZE_32X32, 33);
        static_cast<morpheus::gba::gfx::Sprite4Bpp*>(sprite_4bpp_2.get_sprite_base().get())->
            load_from_array(reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen,
                            15, morpheus::core::gfx::SpriteSize::SIZE_32X32, 33);
    #elif _NDS
        static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sprite_8bpp.get_sprite_base().get())->load_from_array(
                reinterpret_cast<const unsigned short*>(&(test8Tiles[0])), test8TilesLen,
                reinterpret_cast<const unsigned short*>(&(test8Pal[0])), test8PalLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32);
        static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sprite_8bpp_2.get_sprite_base().get())->load_from_array(
                reinterpret_cast<const unsigned short*>(&(test8Tiles[0])), test8TilesLen,
                morpheus::core::gfx::SpriteSize::SIZE_32X32);


        static_cast<morpheus::nds::gfx::Sprite4Bpp*>(sprite_4bpp.get_sprite_base().get())->load_from_array(
                reinterpret_cast<const unsigned short*>(&(test4Tiles[0])), test4TilesLen,
                reinterpret_cast<const unsigned short*>(&(test4Pal[0])), 32, 15,
                  morpheus::core::gfx::SpriteSize::SIZE_32X32);
        static_cast<morpheus::nds::gfx::Sprite4Bpp*>(sprite_4bpp_2.get_sprite_base().get())->load_from_array(
                reinterpret_cast<const unsigned short*>(&(test4Tiles[0])), test4TilesLen, 15,
                morpheus::core::gfx::SpriteSize::SIZE_32X32);
    #endif

    sprite_4bpp.get_sprite_base()->set_priority(3);
    sprite_4bpp_2.get_sprite_base()->set_priority(2);
    sprite_8bpp.get_sprite_base()->set_priority(1);
    sprite_8bpp_2.get_sprite_base()->set_priority(0);

    sprite_4bpp.get_sprite_base()->set_position(0, 0);
    sprite_4bpp_2.get_sprite_base()->set_position(20, 20);
    sprite_8bpp.get_sprite_base()->set_position(35, 35);
    sprite_8bpp_2.get_sprite_base()->set_position(50, 50);

    sprite_4bpp.add_sprite_base_to_main_loop(main_loop);
    sprite_4bpp_2.add_sprite_base_to_main_loop(main_loop);
    sprite_8bpp.add_sprite_base_to_main_loop(main_loop);
    sprite_8bpp_2.add_sprite_base_to_main_loop(main_loop);

    main_loop->game_loop();
}