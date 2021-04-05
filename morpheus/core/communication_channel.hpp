//
// Created by bobby on 01/04/2021.
//

#ifndef MORPHEUS_GBA_TEST_COMMUNICATION_CHANNEL_HPP
#define MORPHEUS_GBA_TEST_COMMUNICATION_CHANNEL_HPP

namespace morpheus {
    namespace core {
        const int CLIENT_ID_GAP = 1;

        enum class ClientId {
            MASTER,
            CLIENT0 = static_cast<int>(ClientId::MASTER) + CLIENT_ID_GAP,
            CLIENT1 = static_cast<int>(ClientId::CLIENT0) + CLIENT_ID_GAP,
            CLIENT2 = static_cast<int>(ClientId::CLIENT1) + CLIENT_ID_GAP,
            INVALID
        };

        const int CLIENT_ID_START = static_cast<int>(ClientId::MASTER);
        const int CLIENT_ID_END = static_cast<int>(ClientId::CLIENT2);

        struct RecievePacket {
            unsigned int data;
            ClientId from;
            bool valid = true;
        };

        class CommunicationChannel {
        public:
            virtual ~CommunicationChannel() = default;

            bool is_master() const {
                return get_client_id() == ClientId::MASTER;
            }

            virtual unsigned int get_available_packets() const = 0;
            virtual ClientId get_client_id() const = 0;
            virtual RecievePacket get_next_recieved_packet() = 0;

            virtual bool is_connected() const = 0;

            virtual bool transmit_data(unsigned short data) = 0;

            virtual void update() = 0;
        };
    }
}

#endif //MORPHEUS_GBA_TEST_COMMUNICATION_CHANNEL_HPP
