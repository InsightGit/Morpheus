//
// Created by bobby on 17/12/2020.
//

#include "sprite.hpp"

morpheus::nds::gfx::Sprite::Sprite(const bool use_sub_display, NdsBlendingController *blending_controller,
                                   NdsMosaicController *mosaic_controller,
                                   const SpriteMapping sprite_mapping, ExtendedPaletteStatus extended_palette) :
                                   SpriteBase(blending_controller, mosaic_controller) {
    if(use_sub_display) {
        m_current_oam = &oamSub;

        vramSetBankD(VRAM_D_SUB_SPRITE);
    } else {
        m_current_oam = &oamMain;

        vramSetBankB(VRAM_B_MAIN_SPRITE);
    }

    bool is_bitmap = sprite_mapping == SpriteMapping_Bmp_1D_128 ||
                     sprite_mapping == SpriteMapping_1D_256 ||
                     sprite_mapping == SpriteMapping_Bmp_2D_128 ||
                     sprite_mapping == SpriteMapping_Bmp_2D_256;
    OamStatus new_oam_status;

    // TODO(Bobby): Investigate bitmap spritemapping preservation while non-bitmap sprite is loaded
    switch(extended_palette) {
        case ExtendedPaletteStatus::NOTNEEDED:
            m_extended_palette = (OAM_STATUS == OamStatus::ENABLED_EXTENDED ||
                                  OAM_STATUS == OamStatus::ENABLED_EXTENDED_BITMAP);
            break;
        case ExtendedPaletteStatus::NEEDED:
            m_extended_palette = true;
            break;
        case ExtendedPaletteStatus::NEEDOFF:
            m_extended_palette = false;
            break;
    }

    if(m_extended_palette) {
        if(is_bitmap) {
            new_oam_status = OamStatus::ENABLED_EXTENDED_BITMAP;
        } else {
            new_oam_status = OamStatus::ENABLED_EXTENDED;
        }
    } else {
        if(is_bitmap) {
            new_oam_status = OamStatus::ENABLED_BITMAP;
        } else {
            new_oam_status = OamStatus::ENABLED;
        }
    }

    if(OAM_STATUS != new_oam_status) {
        OAM_STATUS = new_oam_status;

        oamInit(m_current_oam, sprite_mapping, m_extended_palette);
    }

    m_do_not_free_gfx_pointer = false;
}

morpheus::nds::gfx::Sprite::Sprite(bool use_sub_display, NdsBlendingController *blending_controller,
                                   NdsMosaicController *mosaic_controller, SpriteMapping sprite_mapping,
                                   morpheus::nds::gfx::ExtendedPaletteStatus external_palette,
                                   unsigned short *nds_oam_address,
                                   const morpheus::core::gfx::SpriteSize sprite_size) : Sprite(use_sub_display,
                                                                                               blending_controller,
                                                                                               mosaic_controller,
                                                                                               sprite_mapping,
                                                                                               external_palette) {
    m_do_not_free_gfx_pointer = true;
    m_gfx_pointer = nds_oam_address;

    set_sprite_size(sprite_size);
}

morpheus::nds::gfx::Sprite::~Sprite() {
    nocashMessage(std::string("destroying " + get_position().to_string()).c_str());

    if(m_gfx_pointer != nullptr && !m_do_not_free_gfx_pointer) {
        nocashMessage(std::string("freeing gfx " + std::to_string(reinterpret_cast<unsigned int>(m_gfx_pointer))).c_str());

        oamFreeGfx(m_current_oam, m_gfx_pointer);

        nocashMessage("gfx freed");
    }

    //std::cout << "Destructor called\n";

    if(m_last_used_obj_attr_num != -1) {
        nocashMessage("clearing sprite");

        oamClearSprite(m_current_oam, m_last_used_obj_attr_num);

        nocashMessage("sprite cleared");
    }
    nocashMessage("destructed");
}

void morpheus::nds::gfx::Sprite::allocate_gfx_pointer(const SpriteColorFormat color_format,
                                                      const morpheus::core::gfx::SpriteSize size) {
    set_sprite_size(size);

    if(m_gfx_pointer != nullptr && m_do_not_free_gfx_pointer) {
        //std::cout << "freeing gfx\n";

        oamFreeGfx(m_current_oam, m_gfx_pointer);
    }

    m_gfx_pointer = oamAllocateGfx(m_current_oam, m_sprite_size, color_format);
}

void morpheus::nds::gfx::Sprite::set_sprite_size(morpheus::core::gfx::SpriteSize size) {
    switch(size) {
        case core::gfx::SpriteSize::SIZE_8X8:
            m_sprite_size = SpriteSize_8x8;
            break;
        case core::gfx::SpriteSize::SIZE_16X16:
            m_sprite_size = SpriteSize_16x16;
            break;
        case core::gfx::SpriteSize::SIZE_32X32:
            m_sprite_size = SpriteSize_32x32;
            break;
        case core::gfx::SpriteSize::SIZE_64X64:
            m_sprite_size = SpriteSize_64x64;
            break;
        case core::gfx::SpriteSize::SIZE_16X8:
            m_sprite_size = SpriteSize_16x8;
            break;
        case core::gfx::SpriteSize::SIZE_32X16:
            m_sprite_size = SpriteSize_32x16;
            break;
        case core::gfx::SpriteSize::SIZE_64X32:
            m_sprite_size = SpriteSize_64x32;
            break;
        case core::gfx::SpriteSize::SIZE_8X16:
            m_sprite_size = SpriteSize_8x16;
            break;
        case core::gfx::SpriteSize::SIZE_16X32:
            m_sprite_size = SpriteSize_16x32;
            break;
        case core::gfx::SpriteSize::SIZE_32X64:
            m_sprite_size = SpriteSize_32x64;
            break;
    }
}
