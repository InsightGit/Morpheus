//
// Created by bobby on 01/04/2021.
//

#include "morpheus/gba/serial_communication.hpp"

void morpheus::gba::on_serial_interrupt() {
    // TODO(Bobby): Either get this interrupt working or get rid of it

    nocash_puts("Serial interrupt called");

    if(active_multiplayer_serial_connection != nullptr) {
        active_multiplayer_serial_connection->update();
    }
}

morpheus::gba::MultiplayerSerialCommunication::MultiplayerSerialCommunication(BaudRate baud_rate) {
    active_multiplayer_serial_connection = this;

    // Zero all writable registers as according to GBATEK
    // Set to 1010 1111 1111 1100
    REG_SIOCNT &= 0xAFFC;

    switch (baud_rate) {
        case BaudRate::BAUD_9600:
            REG_SIOCNT |= 0 << BAUD_SHIFT;
            break;
        case BaudRate::BAUD_38400:
            REG_SIOCNT |= 1 << BAUD_SHIFT;
            break;
        case BaudRate::BAUD_57600:
            REG_SIOCNT |= 2 << BAUD_SHIFT;
            break;
        case BaudRate::BAUD_115200:
            REG_SIOCNT |= 3 << BAUD_SHIFT;
            break;
    }

    // Set bits 12 (to 0) and 13 (to 1) to turn on Multiplayer mode
    // in SIOCNT (while enabling serial interrupt in bit 14)
    // 0110 0000 0000 0000
    REG_SIOCNT |= 0x6000;

    // and bits 14 and 15 (both to 0) in RCNT
    REG_RCNT &= 0x3FFF;

    irq_init(isr_master);
    irq_add(II_SERIAL, on_serial_interrupt);
}

bool morpheus::gba::MultiplayerSerialCommunication::transmit_data(unsigned short data) {
    if(is_connected()) {
        m_send_buffer.push_back(data);

        return true;
    } else {
        return false;
    }
}

void morpheus::gba::MultiplayerSerialCommunication::update() {
    if(is_connected()) {
        core::ClientId our_client_id = get_client_id();

        for(int i = core::CLIENT_ID_START; core::CLIENT_ID_END >= i; i += core::CLIENT_ID_GAP) {
            auto current_client_id = static_cast<core::ClientId>(i);
            core::RecievePacket packet;
            bool recieved_data = false;

            packet.from = current_client_id;

            switch (current_client_id) {
                case core::ClientId::MASTER:
                    if(REG_SIOMULTI0 != 0xFFFF) {
                        if(current_client_id != our_client_id) {
                            // Data recieved from master!
                            packet.data = static_cast<unsigned int>(REG_SIOMULTI0);
                        }

                        recieved_data = true;

                        REG_SIOMULTI0 = 0xFFFF;
                    }
                    break;
                case core::ClientId::CLIENT0:
                    if(REG_SIOMULTI1 != 0xFFFF) {
                        if(current_client_id != our_client_id) {
                            // Data recieved from master!
                            packet.data = static_cast<unsigned int>(REG_SIOMULTI1);
                        }

                        recieved_data = true;

                        REG_SIOMULTI1 = 0xFFFF;
                    }
                    break;
                case core::ClientId::CLIENT1:
                    if(REG_SIOMULTI2 != 0xFFFF) {
                        if(current_client_id != our_client_id) {
                            // Data recieved from master!
                            packet.data = static_cast<unsigned int>(REG_SIOMULTI2);
                        }

                        recieved_data = true;

                        REG_SIOMULTI2 = 0xFFFF;
                    }
                    break;
                case core::ClientId::CLIENT2:
                    if(REG_SIOMULTI3 != 0xFFFF) {
                        if(current_client_id != our_client_id) {
                            // Data recieved from master!
                            packet.data = static_cast<unsigned int>(REG_SIOMULTI3);
                        }

                        recieved_data = true;

                        REG_SIOMULTI3 = 0xFFFF;
                    }
                    break;
                case core::ClientId::INVALID:
                    // We should never get to this case
                    break;
            }

            if(recieved_data) {
                if(current_client_id == our_client_id) {
                    m_transmission_in_progress = false;
                } else {
                    m_recieved_buffer.push_back(packet);
                }
            }
        }

        if(!m_transmission_in_progress) {
            send_data_from_queue();
        }
    }
}

bool morpheus::gba::MultiplayerSerialCommunication::send_data_from_queue() {
    if(!m_send_buffer.empty() && is_connected()) {
        // Set Start bit to zero to mark as inactive
        REG_SIOCNT &= ~BUSY_BIT_MASK;

        REG_SIOMLT_SEND = static_cast<unsigned short>(m_send_buffer[0]);

        // Set Start bit to one to mark as busy and initiate transfer
        REG_SIOCNT |= 1 << BUSY_BIT_SHIFT;

        m_send_buffer.pop_front();

        m_transmission_in_progress = true;

        return true;
    } else {
        return false;
    }
}

morpheus::core::ClientId morpheus::gba::MultiplayerSerialCommunication::get_client_id() const {
    // Get Multi-Player ID as according to GBATEK (0: master, 1-3: Clients 1-3)
    unsigned int multiplayer_id = (REG_SIOCNT & MULTIPLAYER_ID_MASK) >> MULTIPLAYER_ID_SHIFT;

    switch (multiplayer_id) {
        case 0:
            return core::ClientId::MASTER;
        case 1:
            return core::ClientId::CLIENT0;
        case 2:
            return core::ClientId::CLIENT1;
        case 3:
            return core::ClientId::CLIENT2;
        default:
            // We should never get to this statement
            return core::ClientId::INVALID;
    }
}
