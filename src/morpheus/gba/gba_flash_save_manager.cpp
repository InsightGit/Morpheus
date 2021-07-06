//
// Created by bobby on 18/04/2021.
//

#include "morpheus/gba/gba_flash_save_manager.hpp"

morpheus::gba::GbaFlashSaveManager::GbaFlashSaveManager(FlashSaveSize flash_save_size) {
    // get flash chip ID as detailed on GBATEK under the GBA Flash ROM section
    *FLASH_DEVICE_REGISTER_1 = 0xAA;
    *FLASH_DEVICE_REGISTER_2 = 0x55;
    *FLASH_DEVICE_REGISTER_1 = 0x90;

    irq_init(nullptr);
    irq_enable(eIrqIndex::II_VBLANK);

    REG_IME = 1;

    // TODO(Bobby): Find a better, shorter delay function to use
    VBlankIntrWait();

    m_device_id = *reinterpret_cast<unsigned char*>(0x0E000001);
    m_manufacturer_id = *reinterpret_cast<unsigned char*>(0x0E000000);

    switch(flash_save_size) {
        case FlashSaveSize::SIZE_AUTO_DETECT:
            // Tests for known and documented (in GBATEK) 128K flash carts. Otherwise, mark it as a 64K flash
            if(((m_device_id == 0x13) && (m_manufacturer_id == 0x62)) ||
               ((m_device_id == 0x09) && (m_manufacturer_id == 0xC2))) {
                m_flash_save_size = FlashSaveSize::SIZE_128_KB;
            } else {
                m_flash_save_size = FlashSaveSize::SIZE_64_KB;
            }
            break;
        default:
            m_flash_save_size = flash_save_size;
            break;
    }

    *FLASH_DEVICE_REGISTER_1 = 0xAA;
    *FLASH_DEVICE_REGISTER_2 = 0x55;
    *FLASH_DEVICE_REGISTER_1 = 0xF0;

    // TODO(Bobby): Find a better, shorter delay function to use
    VBlankIntrWait();

    nocash_puts(std::string("Is Atmel flash: " + std::to_string(is_ateml())).c_str());
}

unsigned int morpheus::gba::GbaFlashSaveManager::load(unsigned char *data, unsigned int len) {
    unsigned int actual_len;

    switch(m_flash_save_size) {
        default:
        case FlashSaveSize::SIZE_AUTO_DETECT:
            // this should never happen but if it does we shouldn't silently succeed
            return 0;
        case FlashSaveSize::SIZE_64_KB:
            actual_len = std::min(len, 65536u);
            break;
        case FlashSaveSize::SIZE_128_KB:
            actual_len = std::min(len, 131072u);

            nocash_puts("read bank switching to zero");
            bank_switch(true);

            break;
    }

    nocash_puts("Printing out:");

    for(unsigned int i = 0; std::min(actual_len, 65536u) > i; ++i) {
        data[i] = FLASH_START[i];

        char temp_array[2] = {data[i], '\0'};

        nocash_puts(temp_array);
    }

    if(actual_len > 65536u) {
        // we need to bank switch before reading
        nocash_puts("read bank switching to one");
        bank_switch(false);

        for(unsigned int i = 65536; actual_len > i; ++i) {
            data[i] = FLASH_START[i - 65536];
        }
    }

    return actual_len;
}

unsigned int morpheus::gba::GbaFlashSaveManager::save(const unsigned char *data, unsigned int len) {
    unsigned int actual_len;

    switch(m_flash_save_size) {
        default:
        case FlashSaveSize::SIZE_AUTO_DETECT:
            // this should never happen but if it does we shouldn't silently succeed
            return 0;
        case FlashSaveSize::SIZE_64_KB:
            actual_len = std::min(len, 65536u);
            break;
        case FlashSaveSize::SIZE_128_KB:
            actual_len = std::min(len, 131072u);

            nocash_puts("write bank switching to zero");
            bank_switch(true);

            break;
    }

    if(actual_len <= 0) {
        return actual_len;
    }

    if(is_ateml()) {
        unsigned int old_ime = REG_IME;

        unsigned int i;

        for(i = 0; std::min(actual_len, 65536u) > i; i += 128) {
            REG_IME = 0;

            *FLASH_DEVICE_REGISTER_1 = 0xAA;
            *FLASH_DEVICE_REGISTER_2 = 0x55;
            *FLASH_DEVICE_REGISTER_1 = 0xA0;

            for(unsigned int i2 = i; std::min(actual_len, i + 128) > i2; ++i2) {
                FLASH_START[i2] = data[i2];
            }

            REG_IME = old_ime;

            // TODO(Bobby): Some sort of assert to assume that all FLASH 128 bytes sector is written
            while(FLASH_START[std::min(actual_len, i + 128)] != data[std::min(actual_len, i + 128)]) {}
        }

        if(actual_len > 65536) {
            bank_switch(false);

            for(i = 65536; actual_len > i; i += 128) {
                REG_IME = 0;

                *FLASH_DEVICE_REGISTER_1 = 0xAA;
                *FLASH_DEVICE_REGISTER_2 = 0x55;
                *FLASH_DEVICE_REGISTER_1 = 0xA0;

                for(unsigned int i2 = i; std::min(actual_len, i + 128) > i2; ++i2) {
                    FLASH_START[i2 - 65536] = data[i2];
                }

                REG_IME = old_ime;

                while(FLASH_START[std::min(actual_len, i + 128) - 65536] != data[std::min(actual_len, i + 128)]) {}
            }
        }

        if(i > actual_len) {
            return i - 128;
        } else {
            return i;
        }
    } else {
        for(unsigned int i = 0; std::min(actual_len, 65536u) > i; ++i) {
            *FLASH_DEVICE_REGISTER_1 = 0xAA;
            *FLASH_DEVICE_REGISTER_2 = 0x55;
            *FLASH_DEVICE_REGISTER_1 = 0xA0;

            FLASH_START[i] = data[i];

            while (FLASH_START[i] != data[i]) {}
        }

        if(actual_len > 65536) {
            bank_switch(false);

            for(unsigned int i = 65536; actual_len > i; ++i) {
                *FLASH_DEVICE_REGISTER_1 = 0xAA;
                *FLASH_DEVICE_REGISTER_2 = 0x55;
                *FLASH_DEVICE_REGISTER_1 = 0xA0;

                FLASH_START[i - 65536] = data[i];

                while (FLASH_START[i - 65536] != data[i]) {}
            }
        }

        return actual_len;
    }
}

void morpheus::gba::GbaFlashSaveManager::bank_switch(bool bank_zero) {
    *FLASH_DEVICE_REGISTER_1 = 0xAA;
    *FLASH_DEVICE_REGISTER_2 = 0x55;
    *FLASH_DEVICE_REGISTER_1 = 0xB0;

    // explicit is better than implicit
    if(bank_zero) {
        FLASH_START[0] = 0;

        while(FLASH_START[0] != 0) {}
    } else {
        FLASH_START[0] = 1;

        while(FLASH_START[0] != 1) {}
    }

    // TODO(Bobby): Find a better, shorter delay function to use
    VBlankIntrWait();
}
