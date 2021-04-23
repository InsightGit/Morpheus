//
// Created by bobby on 01/04/2021.
//

#include "text_base.hpp"


morpheus::core::gfx::TextBase::TextBase(bool affine, unsigned int background_num, unsigned int cbb, unsigned int sbb,
                                        morpheus::core::MainLoop *main_loop) {
    m_affine = affine;
    m_background_num = background_num;
    m_cbb = cbb;
    m_main_loop = main_loop;
    m_sbb = sbb;
}
