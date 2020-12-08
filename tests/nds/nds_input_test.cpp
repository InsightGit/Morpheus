//
// Created by bobby on 04/12/2020.
//

#include <nds/nds.hpp>


/*class MoveableSprite : public morpheus::gba::gfx::Sprite {
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
};*/

int main() {
    std::shared_ptr<morpheus::nds::NdsMainLoop> nds_main_loop(new morpheus::nds::NdsMainLoop());

    /*MoveableSprite test_sprite{};

    test_sprite.load_from_array((unsigned short **) testconfettiTiles, testconfettiTilesLen,
                                (unsigned short **) testconfettiPalLen,
                                testconfettiPalLen, 32, 32, 0);

    test_sprite.set_position(64, 64);

    gba_main_loop->set_root(std::shared_ptr<morpheus::gba::gfx::Sprite>(&test_sprite));*/

    nds_main_loop->game_loop();
}
