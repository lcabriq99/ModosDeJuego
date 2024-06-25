#include <iostream>
#include <MinimalSocket/udp/UdpSocket.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include "functions.h"
#include "estructuras.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <team-name> <this-port>" << endl;
        return 1;
    }

    string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);

    cout << "Creating a UDP socket" << endl;

    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);

    cout << "Socket created" << endl;

    bool success = udp_socket.open();
    if (!success) {
        cout << "Error opening socket" << endl;
        return 1;
    }

    MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};
    cout << "(init " + team_name + "(version 15))";

    udp_socket.sendTo("(init " + team_name + "(version 15))", other_recipient_udp);
    cout << "Init Message sent" << endl;

    std::size_t message_max_size = 1000000;
    cout << "Waiting for a message" << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    Player player{team_name, "", "", false, 0, 0, 0};
    Ball ball{"0", "0", "0", "0"};
    CentroPorteriaDerecha goal_R{"0", "0"};
    CentroPorteriaIzquierda goal_L{"0", "0"};

    player = parseInitialMessage(received_message_content, player);
    cout << player.unum << endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        vector<string> parsed_message = separate_string(received_message_content);

        if (parsed_message[0].find("see") != std::string::npos) {
            vector<string> see_message = separate_string(parsed_message[0]);
            store_data_see(see_message, player, ball, goal_R, goal_L);
        }

        handlePlayerActions(player, ball, goal_R, goal_L, udp_socket, server_udp);
    }

    return 0;
}
