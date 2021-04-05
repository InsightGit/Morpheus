//
// Created by bobby on 02/04/2021.
//

#include <gba/gba.hpp>

class InputNode : public morpheus::core::ControlReciever {
public:
    InputNode(morpheus::gba::MultiplayerSerialCommunication *multiplayer_serial_communication) {
        m_multiplayer_serial_communication = multiplayer_serial_communication;
    }

    void input(const morpheus::core::InputEvent input_event) override {}
    void update(const unsigned char cycle_time) override {
        tte_set_pos(16, 16);

        if(m_multiplayer_serial_communication->is_connected()) {
            std::string connection_name;

            m_multiplayer_serial_communication->transmit_data(cycle_time);

            switch(m_multiplayer_serial_communication->get_client_id()) {
                case morpheus::core::ClientId::MASTER:
                    connection_name = "Master";
                    break;
                case morpheus::core::ClientId::CLIENT0:
                    connection_name = "Client 0";
                    break;
                case morpheus::core::ClientId::CLIENT1:
                    connection_name = "Client 1";
                    break;
                case morpheus::core::ClientId::CLIENT2:
                    connection_name = "Client 2";
                    break;
                case morpheus::core::ClientId::INVALID:
                    connection_name = "An invalid client";
                    break;
            }

            tte_write(std::string("Connected as " + connection_name).c_str());

            tte_set_pos(16, 32);
            tte_write(std::string("<--|" + std::to_string(cycle_time) + "|").c_str());

            if(m_multiplayer_serial_communication->get_available_packets() > 0) {
                morpheus::core::RecievePacket packet = m_multiplayer_serial_communication->get_next_recieved_packet();

                if(packet.valid) {
                    tte_set_pos(16, 48);
                    tte_write(std::string("-->|FROM:" + std::to_string(static_cast<int>(packet.from)) + "|" +
                                          std::to_string(packet.data) + "|").c_str());
                }
            }
        } else {
            tte_write("Not connected");
        }
    }
private:
    morpheus::gba::MultiplayerSerialCommunication *m_multiplayer_serial_communication;
};

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> main_loop(
            new morpheus::gba::GbaMainLoop(morpheus::gba::DebugConsoleMode::ON));
    std::shared_ptr<InputNode> input_node(new InputNode(static_cast<morpheus::gba::MultiplayerSerialCommunication*>(
                                                                      main_loop->get_default_communication_channel())));

    tte_init_se(0, BG_CBB(2) | BG_SBB(2), 0, CLR_YELLOW, 14, nullptr, nullptr);

    //main_loop->get_default_communication_channel()

    tte_write("Morpheus GBA Serial Coms test");

    main_loop->add_control_reciever(input_node);

    main_loop->game_loop();
}
