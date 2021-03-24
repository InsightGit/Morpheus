//
// Created by bobby on 04/12/2020.
//

#include <nds/nds.hpp>

#include "test8_pcx.h"
#include "test8.h"
#include "test4.h"

class SpriteInputBase {
protected:
    morpheus::core::gfx::Vector2 recieved_input(morpheus::core::InputEvent input_event,
                                                morpheus::core::gfx::Vector2 init_position) {
        if(input_event.state == morpheus::core::InputState::HELD || \
               input_event.state == morpheus::core::InputState::DOWN) {

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    init_position = morpheus::core::gfx::Vector2(init_position.get_x(), init_position.get_y() - 5);
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    init_position = morpheus::core::gfx::Vector2(init_position.get_x(), init_position.get_y() + 5);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    init_position = morpheus::core::gfx::Vector2(init_position.get_x() - 5, init_position.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    init_position = morpheus::core::gfx::Vector2(init_position.get_x() + 5, init_position.get_y());
                    break;
                default:
                    break;
            }
        }

        return init_position;
    }
};

class MoveableSprite8 : public morpheus::nds::gfx::Sprite8Bpp, SpriteInputBase {
public:
    MoveableSprite8(bool use_sub_display, bool use_extended_palette) : Sprite8Bpp(use_sub_display, nullptr,
                                                                                  use_extended_palette) {}
protected:
    void input(morpheus::core::InputEvent input_event)override {
        set_position(recieved_input(input_event, get_position()));
    }
};

class MoveableSprite4 : public morpheus::nds::gfx::Sprite4Bpp, SpriteInputBase {
public:
    explicit MoveableSprite4(bool use_sub_display) : Sprite4Bpp(use_sub_display, nullptr) {}
protected:
    void input(morpheus::core::InputEvent input_event)override {
        set_position(recieved_input(input_event, get_position()));
    }
};

int main() {
    std::shared_ptr<morpheus::nds::NdsMainLoop> nds_main_loop(new morpheus::nds::NdsMainLoop(morpheus::nds::DebugConsoleMode::ON_SUB));

    defaultExceptionHandler();

    MoveableSprite8 test_sprite(false, true);
    MoveableSprite8 test2_sprite(false, true);
    MoveableSprite4 test3_sprite(false);
    MoveableSprite4 test4_sprite(false);

    if(!test_sprite.load_from_pcx(test8_pcx, 0)) {
        std::cout << "Couldn't load PCX!\n";
    }

    test2_sprite.load_from_array(reinterpret_cast<const unsigned short*>(&(test8Tiles[0])), test8TilesLen,
                                 0, morpheus::core::gfx::SpriteSize::SIZE_32X32);
    test3_sprite.load_from_array(reinterpret_cast<const unsigned short*>(&(test4Tiles[0])), test4TilesLen,
                                 reinterpret_cast<const unsigned short*>(&(test4Pal[0])), test4PalLen, 0,
                                 morpheus::core::gfx::SpriteSize::SIZE_32X32);
    test4_sprite.load_from_array(reinterpret_cast<const unsigned short*>(&(test4Tiles[0])), test4TilesLen, 0,
                                 morpheus::core::gfx::SpriteSize::SIZE_32X32);

    test_sprite.set_position(0, 0);
    test2_sprite.set_position(20, 20);
    test3_sprite.set_position(35, 35);
    test4_sprite.set_position(50, 50);

    test_sprite.add_child(&test2_sprite);
    test2_sprite.add_child(&test3_sprite);
    test3_sprite.add_child(&test4_sprite);

    nds_main_loop->set_root(std::shared_ptr<morpheus::nds::gfx::Sprite>(&test_sprite));

    nds_main_loop->game_loop();
}
