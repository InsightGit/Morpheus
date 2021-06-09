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


        /// \enum morpheus::core::ClientId
        /// Client ID enum identifying each device on a communication channel
        /// as the master client, or clients 0-2.

        struct RecievePacket {
            unsigned int data;
            ClientId from;
            bool valid = true;
        };


        /// \struct morpheus::core::RecievePacket
        /// Simple struct containing communication channel data information
        /// including the origin's ClientId, the data, and packet validity.

        class CommunicationChannel {
        public:
            /// Destructs the CommunicationChannel object
            virtual ~CommunicationChannel() = default;

            /// Returns whether this Client is the MASTER
            /// (when get_client_id() == ClientId::MASTER), meaning it has to
            /// sometimes do some extra work facilitating communication over
            /// the channel.
            /// \return Whether this CommunicationChannel
            bool is_master() const {
                return get_client_id() == ClientId::MASTER;
            }

            /// Gets the number of available packets to be recieved.
            /// \return The number of available packets to be recieved
            virtual unsigned int get_available_packets() const = 0;

            /// Gets the current ClientId of this client's communication
            /// channel. This ClientId can either represent the Master client,
            /// clients 0-2, or ClientId::INVALID in case of an error.
            /// \return The current ClientId of this client's communication channel
            virtual ClientId get_client_id() const = 0;

            /// Gets the next recieved packet in the RecievePacket buffer.
            /// If the buffer is empty, implementations of this method
            /// should return an invalid packet.
            /// \return the next recieved packet in the RecievePacket buffer
            virtual RecievePacket get_next_recieved_packet() = 0;

            /// Returns whether this client is connected to the communication channel
            /// properly.
            /// \return Whether this client is connected to the communication
            /// channel properly.
            virtual bool is_connected() const = 0;

            /// Transmits an unsigned short of data over the communication
            /// channel.
            /// \param data The unsigned short of data to send
            /// \return Whether the data was successfully transmitted
            virtual bool transmit_data(unsigned short data) = 0;

            /// Updates this communication channel's state and recieved
            /// packets, if the implementation does not support interrupt
            /// handlers.
            virtual void update() = 0;
        };


        /// \class morpheus::core::CommunicationChannel
        ///
        /// A simple base class for handling bi-directional communication
        /// between devices. Right now this is only implemented on the GBA side
        /// with multiplayer serial communication implemented in
        /// morpheus::gba::SerialCommunication, but in the future Wifi support
        /// is planned for the Nintendo DS. CommunicationChannel objects are
        /// usually constructed with the construction of MainLoop objects and
        /// can be accessed by calling MainLoop::get_default_communication_channel.
        /// For an example of this class being used, see ../tests/gba/serial_test/gba_serial_test.cpp
    }
}

#endif //MORPHEUS_GBA_TEST_COMMUNICATION_CHANNEL_HPP
