//
// Created by bobby on 02/04/2021.
//

#include <morpheus/gba/gba.hpp>

class InputNode : public morpheus::core::ControlReciever {
public:
    InputNode(morpheus::gba::MultiplayerSerialCommunication *multiplayer_serial_communication,
              const std::shared_ptr<morpheus::gba::gfx::Text> &text) {
        m_multiplayer_serial_communication = multiplayer_serial_communication;
        m_text = text;
    }

    void input(const morpheus::core::InputEvent input_event) override {}
    void update(const unsigned char cycle_time) override {
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


            m_text->print_at_pos("Connected as " + connection_name, morpheus::core::gfx::Vector2(16, 16));
            m_text->print_at_pos("<--|" + std::to_string(cycle_time) + "|", morpheus::core::gfx::Vector2(16, 32));

            if(m_multiplayer_serial_communication->get_available_packets() > 0) {
                morpheus::core::RecievePacket packet = m_multiplayer_serial_communication->get_next_recieved_packet();

                if(packet.valid) {
                    m_text->print_at_pos("-->|FROM:" + std::to_string(static_cast<int>(packet.from)) + "|" +
                                          std::to_string(packet.data) + "|", morpheus::core::gfx::Vector2(16, 48));
                }
            }
        } else {
            m_text->print_at_pos("Not connected", morpheus::core::gfx::Vector2(16, 16));
        }
    }
private:
    morpheus::gba::MultiplayerSerialCommunication *m_multiplayer_serial_communication;
    std::shared_ptr<morpheus::gba::gfx::Text> m_text;
};

int main() {
    std::shared_ptr<morpheus::gba::GbaMainLoop> main_loop(new morpheus::gba::GbaMainLoop(
            morpheus::gba::DebugConsoleMode::OFF,
            morpheus::core::GbaSaveType::SRAM_32KB));
    std::shared_ptr<morpheus::gba::gfx::Text> text(new morpheus::gba::gfx::Text(false, 0, 2,
                                                                                2, main_loop.get()));
    std::shared_ptr<InputNode> input_node(new InputNode(static_cast<morpheus::gba::MultiplayerSerialCommunication*>(
                                                                      main_loop->get_default_communication_channel()),
                                                        text));

    //tte_init_se(0, BG_CBB(2) | BG_SBB(2), 0, CLR_YELLOW, 14, nullptr, nullptr);

    //main_loop->get_default_communication_channel()

    text->print_at_pos("Morpheus GBA Serial Coms test", morpheus::core::gfx::Vector2(0, 0));

    main_loop->add_control_reciever(input_node);
    main_loop->enable_background(0);

    main_loop->game_loop();
}
