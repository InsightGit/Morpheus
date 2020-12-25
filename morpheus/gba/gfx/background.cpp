//
// Created by bobby on 24/12/2020.
//

#include "background.hpp"


morpheus::gba::gfx::Background::Background(unsigned int background_num) {
    m_background_num = std::min(3u, background_num);

    switch (m_background_num) {
        case 0:
            m_bg_control_register = REG_BG0CNT;
            break;
        case 1:
            m_bg_control_register = REG_BG1CNT;
            break;
        case 2:
            m_bg_control_register = REG_BG2CNT;
            break;
        case 3:
            m_bg_control_register = REG_BG3CNT;
            break;
    }
}
