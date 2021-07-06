//
// Created by bobbyy on 5/19/21.
//

#ifndef HAYAI_PLAYER_HUD_HPP
#define HAYAI_PLAYER_HUD_HPP

#ifdef _GBA
#include <morpheus/gba/gba.hpp>
#elif _NDS
#include <morpheus/nds/nds.hpp>
#endif

#include <iomanip>

#include "level.hpp"

#include "player_hud.h"

namespace hayai {
    class PlayerHud {
    public:
        static const std::vector<unsigned int> NUMBER_ZERO_TILES;
        static const std::vector<unsigned int> NUMBER_ONE_TILES;
        static const std::vector<unsigned int> NUMBER_TWO_TILES;
        static const std::vector<unsigned int> NUMBER_THREE_TILES;
        static const std::vector<unsigned int> NUMBER_FOUR_TILES;
        static const std::vector<unsigned int> NUMBER_FIVE_TILES;
        static const std::vector<unsigned int> NUMBER_SIX_TILES;
        static const std::vector<unsigned int> NUMBER_SEVEN_TILES;
        static const std::vector<unsigned int> NUMBER_EIGHT_TILES;
        static const std::vector<unsigned int> NUMBER_NINE_TILES;

        PlayerHud(std::shared_ptr<morpheus::core::MainLoop> main_loop);

        virtual ~PlayerHud() = default;

        unsigned int get_coin_number() const {
            return m_coin_number;
        }

        void set_coin_number(const unsigned int coin_number) {
            m_coin_number = coin_number;
        }

        unsigned int get_health_number() const {
            return m_health_number;
        }

        void set_health_number(const unsigned int health_number) {
            m_health_number = health_number;
        }

        void update_hud_numbers();
    private:
        static const morpheus::core::gfx::Vector2 COIN_NUMBER_POSITION;
        static const unsigned int DEFAULT_COIN_VALUE;
        static const unsigned int DEFAULT_HEART_VALUE;
        static const morpheus::core::gfx::Vector2 HEALTH_NUMBER_POSITION;
        static const unsigned int HUD_BACKGROUND_NUM;
        static const morpheus::core::gfx::Vector2 HUD_POSITION;
        static const morpheus::core::gfx::Vector2 HUD_SIZE;
        static const unsigned int MAX_HUD_DIGITS;
        static const bool USE_NDS_SUBSCREEN;

        std::vector<unsigned int> get_digit_tile_vector(unsigned int digit) const {
            switch(digit) {
                case 0:
                    return NUMBER_ZERO_TILES;
                case 1:
                    return NUMBER_ONE_TILES;
                case 2:
                    return NUMBER_TWO_TILES;
                case 3:
                    return NUMBER_THREE_TILES;
                case 4:
                    return NUMBER_FOUR_TILES;
                case 5:
                    return NUMBER_FIVE_TILES;
                case 6:
                    return NUMBER_SIX_TILES;
                case 7:
                    return NUMBER_SEVEN_TILES;
                case 8:
                    return NUMBER_EIGHT_TILES;
                case 9:
                    return NUMBER_NINE_TILES;
                default:
                    return {};
            }
        }

        void print_number_at(const unsigned int number, const morpheus::core::gfx::Vector2 position);

        unsigned int m_coin_number = DEFAULT_COIN_VALUE;
        unsigned int m_health_number = DEFAULT_HEART_VALUE;
        std::unique_ptr<morpheus::core::gfx::TiledBackgroundBase> m_hud_background;
        std::unique_ptr<morpheus::core::gfx::Window> m_hud_window;
        std::unique_ptr<morpheus::core::gfx::Window> m_out_window;
        unsigned int m_past_coin_number = 0;
        unsigned int m_past_health_number = 0;
    };
}

#endif //HAYAI_PLAYER_HUD_HPP
