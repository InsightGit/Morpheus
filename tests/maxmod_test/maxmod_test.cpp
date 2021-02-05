//
// Created by bobby on 02/02/2021.
//

#ifdef _GBA
#include <gba/gba.hpp>
#elif _NDS
#include <nds/nds.hpp>
#endif

#include "soundbank.h"
#include "soundbank_bin.h"

#pragma GCC diagnostic warning "-Wswitch"

class SfxControls : public morpheus::core::Node {
public:
    SfxControls(std::shared_ptr<morpheus::core::audio::MaxModSfx> sfx) {
        m_sfx = sfx;
    }

    virtual void input(morpheus::core::InputEvent input_event)override {
        if(input_event.state == morpheus::core::InputState::DOWN ||
           input_event.state == morpheus::core::InputState::HELD) {
            switch(input_event.button) {
                case morpheus::core::InputButton::A:
                    m_sfx->start_effect(false);
                    break;
                case morpheus::core::InputButton::B:
                    m_sfx->stop_effect();
                    break;
            }
        }
    }

    void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority) override {}
private:
    std::shared_ptr<morpheus::core::audio::MaxModSfx> m_sfx;
};

int main() {
    std::shared_ptr<morpheus::core::MainLoop> main_loop(morpheus::utils::construct_appropriate_main_loop());
    std::shared_ptr<morpheus::core::audio::MaxModMusic> max_mod_music(
            morpheus::utils::construct_appropriate_max_mod_music(
                     MOD_EXAMPLE, const_cast<void *>(static_cast<const void*>(&soundbank_bin)), 4));
    std::shared_ptr<morpheus::core::audio::MaxModSfx> max_mod_sfx(
                                                    morpheus::utils::construct_appropriate_max_mod_sfx(SFX_EXAMPLESFX));
    std::shared_ptr<morpheus::core::Node> sfx_controls(new SfxControls(max_mod_sfx));

    max_mod_music->play_music(true);

    main_loop->set_root(sfx_controls);

    std::cout << "sfx controls\n";

    main_loop->game_loop();
}
