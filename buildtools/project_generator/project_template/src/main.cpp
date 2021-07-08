#ifdef _GBA
#include <morpheus/gba/gba.hpp>
#elif _NDS
#include <morpheus/nds/nds.hpp>
#else
#error Unsupported platform!
#endif

// Generated SpriteBase image data includes
#include "sprite_4bpp.h"
#include "sprite_8bpp.h"

// Generated TiledBaseBackground data includes
#include "cloud_tilemap.h"

// Generated MaxMod sound bank includes
#include "soundbank.h"
#include "soundbank_bin.h"

class SampleControlReciever : public morpheus::core::ControlReciever {
public:
    SampleControlReciever(const std::shared_ptr<morpheus::core::audio::MaxModSfx> &sfx,
                          const std::shared_ptr<morpheus::core::gfx::SpriteBase> &sprite) {
        m_sfx = sfx;
        m_sprite = sprite;
    }

    void input(const morpheus::core::InputEvent input_event) override {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            morpheus::core::gfx::Vector2 player_pos = m_sprite->get_position();

            switch (input_event.button) {
                case morpheus::core::InputButton::A:
                    m_sfx->start_effect(false);
                    break;
                case morpheus::core::InputButton::DPADUP:
                    player_pos = morpheus::core::gfx::Vector2(player_pos.get_x(), player_pos.get_y() - 10);
                    break;
                case morpheus::core::InputButton::DPADLEFT:
                    player_pos = morpheus::core::gfx::Vector2(player_pos.get_x() - 10, player_pos.get_y());
                    break;
                case morpheus::core::InputButton::DPADRIGHT:
                    player_pos = morpheus::core::gfx::Vector2(player_pos.get_x() + 10, player_pos.get_y());
                    break;
                case morpheus::core::InputButton::DPADDOWN:
                    player_pos = morpheus::core::gfx::Vector2(player_pos.get_x(), player_pos.get_y() + 10);
                    break;
                default:
                    break;
            }


            m_sprite->set_position(player_pos);
        }
    }

    void update(const unsigned char cycle_time) override {}
private:
    std::shared_ptr<morpheus::core::audio::MaxModSfx> m_sfx;
    std::shared_ptr<morpheus::core::gfx::SpriteBase> m_sprite;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());

    std::shared_ptr<morpheus::core::gfx::TextBase> hello_world_text(morpheus::utils::construct_appropriate_text(
            false, 0, 0, 5, main_loop.get()));
    std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> sample_background(morpheus::utils::
                                                                                construct_appropriate_tiled_background_4bpp(false, 1, main_loop->get_blending_controller(),
                                                                                                                            main_loop->get_mosaic_controller(), main_loop.get(),
                                                                                                                            3, 10));
    std::shared_ptr<morpheus::core::gfx::SpriteBase> sample_sprite(morpheus::utils::construct_appropriate_sprite_8bpp(
            false, main_loop->get_blending_controller(), main_loop->get_mosaic_controller()));
    std::shared_ptr<morpheus::core::audio::MaxModSfx> sample_sfx(morpheus::utils::construct_appropriate_max_mod_sfx(
            SFX_TEST_SOUND, const_cast<void *>(static_cast<const void*>(&soundbank_bin)), 4));

    std::shared_ptr<SampleControlReciever> sample_control_reciever(new SampleControlReciever(sample_sfx,
                                                                                             sample_sprite));

    #ifdef _GBA
        auto gba_sprite = static_cast<morpheus::gba::gfx::Sprite*>(sample_sprite.get());

        gba_sprite->load_from_array(reinterpret_cast<const unsigned short *>(sprite_8bppTiles), sprite_8bppTilesLen,
                                    sprite_8bppPal, sprite_8bppPalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32, 0);
    #elif _NDS
        auto nds_sprite = static_cast<morpheus::nds::gfx::Sprite8Bpp*>(sample_sprite.get());

        nds_sprite->load_from_array(reinterpret_cast<const unsigned short *>(sprite_8bppTiles), sprite_8bppTilesLen,
                                    sprite_8bppPal, sprite_8bppPalLen, morpheus::core::gfx::SpriteSize::SIZE_32X32);
    #endif

    hello_world_text->print_at_pos("Hello World!", morpheus::core::gfx::Vector2(72, 8));

    sample_background->load_from_array(cloud_tilemapTiles, cloud_tilemapTilesLen, cloud_tilemapPal,
                                       cloud_tilemapPalLen, cloud_tilemapMap, cloud_tilemapMapLen,
                                       morpheus::core::gfx::TiledBackgroundSize::BG_32x32);

    sample_sprite->set_position(morpheus::core::gfx::Vector2(64, 64));

    main_loop->add_sprite(sample_sprite);
    main_loop->add_control_reciever(sample_control_reciever);

    main_loop->game_loop();
}
