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

namespace morpheus {
    namespace utils {
        class BackgroundTestControls : public morpheus::core::Node {
        public:
            BackgroundTestControls(std::shared_ptr<morpheus::core::gfx::TiledBackgroundBase> background) {
                m_background = background;
            }

            void draw_node(std::vector<void *>obj_attr_buffer, int obj_attr_num, int priority)override {}

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

        static morpheus::core::MainLoop* construct_appropriate_main_loop() {
            #ifdef _GBA
                return new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::OFF);
            #elif _NDS
                return new morpheus::nds::NdsMainLoop(morpheus::nds::DebugConsoleMode::ON_SUB);
            #endif

            return nullptr;
        }

        static morpheus::core::audio::MaxModMusic *construct_appropriate_max_mod_music(void *sound_bank,
                                                                                       int num_of_channels,
                                                                                       int sound_bank_ref_num) {
            #ifdef _GBA
                return new morpheus::gba::audio::GbaMaxModMusic(sound_bank, num_of_channels, sound_bank_ref_num);
            #elif _NDS
                return new morpheus::nds::audio::NdsMaxModMusic(sound_bank, sound_bank_ref_num);
            #endif

            return nullptr;
        }
    }
}


#endif //MORPHEUS_GBA_TEST_UTILS_HPP
