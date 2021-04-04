#include <core/core.hpp>
#include <gba/gba.hpp>

//#include "testconfetti.h"
#include "test4.h"
#include "test8.h"

class InputBase {
protected:
    morpheus::core::gfx::Vector2 input_routine(morpheus::core::InputEvent input_event,
                                               morpheus::core::gfx::Vector2 position) {
        if(input_event.state == morpheus::core::InputState::HELD || \
               input_event.state == morpheus::core::InputState::DOWN) {

            nocash_puts("input detected");

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() - 5);
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    position = morpheus::core::gfx::Vector2(position.get_x(), position.get_y() + 5);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    position = morpheus::core::gfx::Vector2(position.get_x() - 5, position.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    position = morpheus::core::gfx::Vector2(position.get_x() + 5, position.get_y());
                    break;
                default:
                    break;
            }
        }

        return position;
    }
};

class MoveableSprite8 : public morpheus::gba::gfx::Sprite8Bpp, InputBase {
public:
    MoveableSprite8() : morpheus::gba::gfx::Sprite8Bpp(nullptr, nullptr) {}
protected:
    void input(morpheus::core::InputEvent input_event)override {
        set_position(input_routine(input_event, get_position()));
    }
};

class MoveableSprite4 : public morpheus::gba::gfx::Sprite4Bpp, InputBase {
public:
    MoveableSprite4() : morpheus::gba::gfx::Sprite4Bpp(nullptr, nullptr) {}
protected:
    void input(morpheus::core::InputEvent input_event)override {
        set_position(input_routine(input_event, get_position()));
    }
};

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> gba_main_loop(
            new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::USE_DEFAULT));

    MoveableSprite4 test_sprite4{};
    MoveableSprite4 test_sprite4_2{};
    MoveableSprite8 test_sprite8{};
    MoveableSprite8 test_sprite8_2{};

    test_sprite8.load_from_array(reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                                 reinterpret_cast<const unsigned short *>(test8Pal), test8PalLen,
                                morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    test_sprite8_2.load_from_array(reinterpret_cast<const unsigned short *>(test8Tiles), test8TilesLen,
                                   morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);

    test_sprite4.load_from_array(reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen,
                                 reinterpret_cast<const unsigned short *>(test4Pal), 32,
                                 15, morpheus::core::gfx::SpriteSize::SIZE_32X32, 33);
    test_sprite4_2.load_from_array(reinterpret_cast<const unsigned short *>(test4Tiles), test4TilesLen,
                                   15, morpheus::core::gfx::SpriteSize::SIZE_32X32, 33);

    test_sprite8.set_position(64, 64);
    test_sprite8_2.set_position(80, 80);
    test_sprite4.set_position(96, 96);
    test_sprite4_2.set_position(112, 112);

    gba_main_loop->add_sprite(std::shared_ptr<morpheus::gba::gfx::Sprite>(&test_sprite8));
    gba_main_loop->add_sprite(std::shared_ptr<morpheus::gba::gfx::Sprite>(&test_sprite8_2));
    gba_main_loop->add_sprite(std::shared_ptr<morpheus::gba::gfx::Sprite>(&test_sprite4));
    gba_main_loop->add_sprite(std::shared_ptr<morpheus::gba::gfx::Sprite>(&test_sprite4_2));

    test_sprite8.set_priority(3);
    test_sprite8_2.set_priority(2);
    test_sprite4.set_priority(1);
    test_sprite4_2.set_priority(0);

    gba_main_loop->game_loop();
}
