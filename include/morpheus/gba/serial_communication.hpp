//
// Created by bobby on 01/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_SERIAL_COMMUNICATION_HPP
#define MORPHEUS_GBA_TEST_SERIAL_COMMUNICATION_HPP

#include <deque>

#include <tonc.h>

#include <morpheus/core/communication_channel.hpp>

namespace morpheus {
    namespace gba {
        enum class BaudRate {
            BAUD_9600, ///< 9600 bits/sec
            BAUD_38400, ///< 38400 bits/sec
            BAUD_57600, ///< 57600 bits/sec
            BAUD_115200 ///< 115200 bits/sec
        };

        /// \enum morpheus::gba::BaudRate
        /// An enum class representing a certain baud rate over GBA's serial
        /// port (aka "link cable")

        class MultiplayerSerialCommunication;

        static MultiplayerSerialCommunication *active_multiplayer_serial_connection;

        void on_serial_interrupt();

        class MultiplayerSerialCommunication : public morpheus::core::CommunicationChannel {
        public:
            /// Constructs a MultiplayerSerialCommunication object, with a
            /// given BaudRate.
            /// \param baud_rate The baud rate for this multiplayer-mode serial
            /// communication channel to use
            explicit MultiplayerSerialCommunication(BaudRate baud_rate = BaudRate::BAUD_9600);

            /// Destructs the MultiplayerSerialCommunication object.
            ~MultiplayerSerialCommunication() override {
                active_multiplayer_serial_connection = nullptr;
            }

            /// \return The current baud rate of this GBA multiplayer-mode
            /// serial connection.
            BaudRate get_baud_rate() const {
                return m_baud_rate;
            }

            bool is_connected() const override {
                // Check SIOCNT Bit 3 to verify connection as specified in GBATEK
                return static_cast<bool>((REG_SIOCNT & SD_TERMINAL_MASK) >> SD_TERMINAL_SHIFT);
            };

            unsigned int get_available_packets() const override {
                return m_recieved_buffer.size();
            }

            core::RecievePacket get_next_recieved_packet() override {
                core::RecievePacket return_value;

                if(m_recieved_buffer.empty()) {
                    return_value.valid = false;
                } else {
                    return_value = m_recieved_buffer[0];

                    m_recieved_buffer.pop_front();
                }

                return return_value;
            }

            core::ClientId get_client_id()const override;
            bool transmit_data(unsigned short data)override;
            void update()override;
        private:
            const unsigned int BAUD_SHIFT = 0;

            const unsigned int BUSY_BIT_MASK = 0x0080;
            const unsigned int BUSY_BIT_SHIFT = 7;

            const unsigned int SD_TERMINAL_MASK = 0x0008;
            const unsigned int SD_TERMINAL_SHIFT = 3;

            const unsigned int SI_TERMINAL_MASK = 0x0004;
            const unsigned int SI_TERMINAL_SHIFT = 2;

            const unsigned int MULTIPLAYER_ID_MASK = 0x0030;
            const unsigned int MULTIPLAYER_ID_SHIFT = 4;

            BaudRate m_baud_rate;
            std::deque<core::RecievePacket> m_recieved_buffer;
            std::deque<unsigned int> m_send_buffer;
            bool m_transmission_in_progress = false;

            bool send_data_from_queue();
        };


        /// \class morpheus::gba::MultiplayerSerialCommunication
        /// The GBA multiplayer-mode serial (aka "link cable") implementation
        /// of morpheus::core::CommunicationChannel, allowing bi-directional
        /// communication between GBAs useful for multiplayer games or save data
        /// transferring. This class specifically implements multiplayer-mode
        /// serial communication, as detailed on GBATek
        /// [here](https://problemkaputt.de/gbatek.htm#siomultiplayermode).
    }
}

#endif //MORPHEUS_GBA_TEST_SERIAL_COMMUNICATION_HPP
