//
// Created by bobby on 04/12/2020.
//

#include <nds/nds.hpp>

#include "test8_pcx.h"
#include "test8.h"

class MoveableSprite : public morpheus::nds::gfx::Sprite8Bpp {
public:
    MoveableSprite(bool use_sub_display, bool use_extended_palette) : Sprite8Bpp(use_sub_display,
                                                                                 use_extended_palette) {}
protected:
    void input(morpheus::core::InputEvent input_event)override {
        if(input_event.state == morpheus::core::InputState::HELD || \
               input_event.state == morpheus::core::InputState::DOWN) {
            morpheus::core::gfx::Vector2 vector_2 = get_position();

            switch(input_event.button) {
                case morpheus::core::InputButton::DPADUP:
                    vector_2 = morpheus::core::gfx::Vector2(vector_2.get_x(), vector_2.get_y() - 5);
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    vector_2 = morpheus::core::gfx::Vector2(vector_2.get_x(), vector_2.get_y() + 5);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    vector_2 = morpheus::core::gfx::Vector2(vector_2.get_x() - 5, vector_2.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    vector_2 = morpheus::core::gfx::Vector2(vector_2.get_x() + 5, vector_2.get_y());
                    break;
                default:
                    break;
            }

            set_position(vector_2);
        }
    }
};

int main() {
    std::shared_ptr<morpheus::nds::NdsMainLoop> nds_main_loop(new morpheus::nds::NdsMainLoop(morpheus::nds::DebugConsoleMode::ON_MAIN));

    defaultExceptionHandler();

    MoveableSprite test_sprite(true, false);
    MoveableSprite test2_sprite(true, false);

    if(!test_sprite.load_from_pcx(test8_pcx, 0)) {
        std::cout << "Couldn't load PCX!\n";
    }

    test2_sprite.load_from_array(reinterpret_cast<const unsigned short*>(&(test8Tiles[0])), 32, 32);

    test_sprite.set_position(0, 0);
    test2_sprite.set_position(64, 64);

    test_sprite.add_child(&test2_sprite);

    nds_main_loop->set_root(std::shared_ptr<morpheus::nds::gfx::Sprite>(&test_sprite));

    nds_main_loop->game_loop();
}
