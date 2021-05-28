//
// Created by bobbyy on 5/19/21.
//

#include "player_hud.hpp"

const std::vector<unsigned int> hayai::PlayerHud::NUMBER_ZERO_TILES = {0x40, 0x41, 0x50, 0x51};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_ONE_TILES = {0x42, 0x43, 0x52, 0x53};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_TWO_TILES = {0x44, 0x45, 0x54, 0x55};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_THREE_TILES = {0x46, 0x47, 0x56, 0x57};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_FOUR_TILES = {0x48, 0x49, 0x58, 0x59};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_FIVE_TILES = {0x4A, 0x4B, 0x5A, 0x5B};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_SIX_TILES = {0x4C, 0x4D, 0x5C, 0x5D};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_SEVEN_TILES = {0x4E, 0x4F, 0x5E, 0x5F};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_EIGHT_TILES = {0x60, 0x61, 0x70, 0x71};
const std::vector<unsigned int> hayai::PlayerHud::NUMBER_NINE_TILES = {0x62, 0x63, 0x72, 0x73};

const morpheus::core::gfx::Vector2 hayai::PlayerHud::COIN_NUMBER_POSITION = morpheus::core::gfx::Vector2(96, 8);
const unsigned int hayai::PlayerHud::DEFAULT_COIN_VALUE = 0;
const unsigned int hayai::PlayerHud::DEFAULT_HEART_VALUE = 3;
const morpheus::core::gfx::Vector2 hayai::PlayerHud::HEALTH_NUMBER_POSITION = morpheus::core::gfx::Vector2(32, 8);
const unsigned int hayai::PlayerHud::HUD_BACKGROUND_NUM = 0;
const morpheus::core::gfx::Vector2 hayai::PlayerHud::HUD_POSITION = morpheus::core::gfx::Vector2(0,  0);
const morpheus::core::gfx::Vector2 hayai::PlayerHud::HUD_SIZE = morpheus::core::gfx::Vector2(136, 32);
const unsigned int hayai::PlayerHud::MAX_HUD_DIGITS = 3;
const bool hayai::PlayerHud::USE_NDS_SUBSCREEN = true;

hayai::PlayerHud::PlayerHud(std::shared_ptr<morpheus::core::MainLoop> main_loop) {
    m_hud_background.reset(morpheus::utils::construct_appropriate_tiled_background_8bpp(
                                                                                false,
                                                                                HUD_BACKGROUND_NUM,
                                                                                main_loop->get_blending_controller(),
                                                                                main_loop->get_mosaic_controller(),
                                                                                main_loop.get(), 3,
                                                                                15, USE_NDS_SUBSCREEN));

    m_hud_window.reset(morpheus::utils::construct_appropriate_window(morpheus::core::gfx::WindowType::WINDOW_0,
                                                                     main_loop, USE_NDS_SUBSCREEN));
    m_out_window.reset(morpheus::utils::construct_appropriate_window(morpheus::core::gfx::WindowType::WINDOW_OUT,
                                                                     main_loop, USE_NDS_SUBSCREEN));

    #ifdef _GBA
        bool gba = true;
    #else
        bool gba = false;
    #endif

    if(gba || !USE_NDS_SUBSCREEN) {
        m_hud_window->add_background(HUD_BACKGROUND_NUM);
        m_out_window->add_background(Level::MAIN_LEVEL_BACKGROUND_NUM);

        m_out_window->enable_objects();

        m_hud_window->enable_blending();
        m_out_window->enable_blending();

        m_hud_window->set_window_rect({
            .bottom = static_cast<unsigned char>(HUD_POSITION.get_y() + HUD_SIZE.get_y()),
            .left = static_cast<unsigned char>(HUD_POSITION.get_x()),
            .right = static_cast<unsigned char>(HUD_POSITION.get_x() + HUD_SIZE.get_x()),
            .top = static_cast<unsigned char>(HUD_POSITION.get_y()),
        });

        m_hud_window->enable_window();
        m_out_window->enable_window();

        m_hud_background->load_from_array(player_hudMap, player_hudMapLen,
                                          morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
    } else {
        m_hud_background->load_from_array(player_hudTiles, player_hudTilesLen, player_hudPal, player_hudPalLen,
                                          player_hudMap, player_hudMapLen,
                                          morpheus::core::gfx::TiledBackgroundSize::BG_32x32);
    }
}

void hayai::PlayerHud::print_number_at(const unsigned int number, const morpheus::core::gfx::Vector2 position) {
    std::vector<unsigned int> digits;
    unsigned int digit_num = 0;
    unsigned int working_number = number;
    morpheus::core::gfx::Vector2 working_position = position;

    if(working_number == 0) {
        digits.push_back(0);
    } else {
        while(working_number != 0 && digit_num < MAX_HUD_DIGITS) {
            working_number /= 10;

            if(working_number == 0) {
                digits.push_back(number % 10);
            } else {
                digits.push_back(working_number);
            }

            ++digit_num;
        }
    }

    for(unsigned int i = 0; digits.size() > i; ++i) {
        std::vector<unsigned int> digit_tiles = get_digit_tile_vector(digits[i]);

        for(unsigned int i2 = 0; digit_tiles.size() > i2; ++i2) {
            morpheus::core::gfx::Vector2 tile_position;

            // TODO(Bobby): there must be a better way to do this...
            switch(i2) {
                case 0: {
                    tile_position = morpheus::core::gfx::Vector2(0, 0);
                    break;
                }
                case 1: {
                    tile_position = morpheus::core::gfx::Vector2(8, 0);
                    break;
                }
                case 2: {
                    tile_position = morpheus::core::gfx::Vector2(0, 8);
                    break;
                }
                case 3: {
                    tile_position = morpheus::core::gfx::Vector2(8, 8);
                    break;
                }
            }

            m_hud_background->set_tile_id_at_position(working_position + tile_position, digit_tiles[i2]);
            m_hud_background->set_tile_id_at_position(working_position + tile_position, digit_tiles[i2]);
        }

        working_position = working_position + morpheus::core::gfx::Vector2(16, 0);
    }
}

void hayai::PlayerHud::update_hud_numbers() {
    if(m_past_coin_number != m_coin_number) {
        m_past_coin_number = m_coin_number;

        print_number_at(m_coin_number, COIN_NUMBER_POSITION);
    }

    if(m_past_health_number != m_health_number) {
        m_past_health_number = m_health_number;

        print_number_at(m_health_number, HEALTH_NUMBER_POSITION);
    }
}
