//
// Created by bobby on 23/01/2021.
//

#ifndef MORPHEUS_GBA_TEST_UTILS_HPP
#define MORPHEUS_GBA_TEST_UTILS_HPP

#ifdef _GBA
    #include <gba/gba.hpp>
#elif _NDS
    #include <nds/nds.hpp>
#else
    #error Unsupported Platform!
#endif

#pragma GCC diagnostic ignored "-Wunused-function"

namespace morpheus {
    namespace utils {
        class BackgroundTestControls : public morpheus::core::Node {
        public:
            BackgroundTestControls(std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background) {
                m_background = background;
            }

            void draw_node(std::vector<void *> &obj_attr_buffer, int obj_attr_num, int priority)override {}

            virtual void input(morpheus::core::InputEvent input_event)override {
                scroll_background(input_event);
            }

            void change_background(const std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> new_background) {
                m_background = new_background;
            }

        protected:
            void scroll_background(morpheus::core::InputEvent input_event) {
                if(input_event.state == morpheus::core::InputState::DOWN ||
                   input_event.state == morpheus::core::InputState::HELD) {
                    morpheus::core::gfx::Vector2 scroll_pos = m_background->get_scroll();

                    switch(input_event.button) {
                        case morpheus::core::InputButton::DPADUP:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() - 10);
                            break;
                        case morpheus::core::InputButton::DPADLEFT:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() - 10, scroll_pos.get_y());
                            break;
                        case morpheus::core::InputButton::DPADRIGHT:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x() + 10, scroll_pos.get_y());
                            break;
                        case morpheus::core::InputButton::DPADDOWN:
                            scroll_pos = morpheus::core::gfx::Vector2(scroll_pos.get_x(), scroll_pos.get_y() + 10);
                            break;
                        default:
                            break;
                    }

                    m_background->set_scroll(scroll_pos);
                }
            }
        private:
            std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> m_background;
        };

        static morpheus::core::MainLoop* construct_appropriate_main_loop(bool enable_screen_debug_gba = false,
                                                                         bool enable_screen_debug_nds = true) {
            #ifdef _GBA
                if(enable_screen_debug_gba) {
                    return new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::ON);
                } else {
                    return new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::NOCASH_ONLY);
                }
            #elif _NDS
                if(enable_screen_debug_nds) {
                    return new morpheus::nds::NdsMainLoop(morpheus::nds::DebugConsoleMode::ON_SUB);
                } else {
                    return new morpheus::nds::NdsMainLoop(morpheus::nds::DebugConsoleMode::OFF);
                }
            #endif

            return nullptr;
        }

        static morpheus::core::audio::MaxModMusic *construct_appropriate_max_mod_music(
                                                                                   int sound_bank_ref_num,
                                                                                   void *sound_bank = nullptr,
                                                                                   unsigned char num_of_channels = -1) {
            #ifdef _GBA
                return new morpheus::gba::audio::GbaMaxModMusic(sound_bank_ref_num, sound_bank, num_of_channels);
            #elif _NDS
                return new morpheus::nds::audio::NdsMaxModMusic(sound_bank_ref_num, sound_bank);
            #endif

            return nullptr;
        }

        static morpheus::core::audio::MaxModSfx *construct_appropriate_max_mod_sfx(
                int sound_bank_ref_num,
                void *sound_bank = nullptr,
                unsigned char num_of_channels = -1) {
            #ifdef _GBA
                return new morpheus::gba::audio::GbaMaxModSfx(sound_bank_ref_num, sound_bank, num_of_channels);
            #elif _NDS
                return new morpheus::nds::audio::NdsMaxModSfx(sound_bank_ref_num, sound_bank);
            #endif

            return nullptr;
        }

        //static morpheus::core::gfx::TiledBackgroundBase *construct_appropriate_tiled_background()
    }
}


#endif //MORPHEUS_GBA_TEST_UTILS_HPP
