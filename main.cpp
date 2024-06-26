#include <iostream>
#include <MinimalSocket/udp/UdpSocket.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>

// Include headers
#include "functions.h"
#include "structs.h"

// Move command function (same as before)
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient)
{
    struct Posicion
    {
        int x;
        int y;
    };

    vector<Posicion> posiciones = {
        {-50, 0}, // goalkeeper
        {-40, -10}, {-35, -28},
        {-40, 10}, {-35, 28},
        {-25, 11}, {-8, 20},
        {-25, -11}, {-5, 0},
        {-15, 0}, {-8, -20}
    };

    Posicion myPos = posiciones[player.unum - 1];

    auto moveCommand = "(move " + std::to_string(myPos.x) + " " + std::to_string(myPos.y) + ")";
    udp_socket.sendTo(moveCommand, recipient);
    std::cout << "Move command sent: " << moveCommand << std::endl;
}

// Main function
int main(int argc, char *argv[])
{
    // Check if the number of arguments is correct
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <team-name> <this-port>" << std::endl;
        return 1;
    }

    // Get the team name and the port
    std::string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);

    std::cout << "Creating a UDP socket" << std::endl;
    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);
    std::cout << "Socket created" << std::endl;

    bool success = udp_socket.open();

    if (!success)
    {
        std::cout << "Error opening socket" << std::endl;
        return 1;
    }

    MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};
    std::cout << "(init " + team_name + "(version 15))" << std::endl;

    udp_socket.sendTo("(init " + team_name + "(version 15))", other_recipient_udp);
    std::cout << "Init Message sent" << std::endl;

    std::size_t message_max_size = 1000000;
    std::cout << "Waiting for a message" << std::endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    // Update UDP port provided by the other UDP
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    // Create objects
    Player player{team_name, "", "", false, 0, 0, 0};
    Ball ball{"0", "0", "0", "0"};
    Goal own_goal{"0", "0", "init"};
    Goal opponent_goal{"0", "0", "init"};

    // Parse the initial message
    player = parseInitialMessage(received_message_content, player);
    std::cout << "Player number: " << player.unum << std::endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);

    // Configure the goals
    if (player.side == "r")
    {
        opponent_goal.side = "l";
        own_goal.side = "r";
    }
    else
    {
        opponent_goal.side = "r";
        own_goal.side = "l";
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        std::vector<std::string> parsed_message = separate_string(received_message_content);

        // Search for see message
        if (parsed_message[0].find("see") <= 5)
        {
            std::vector<std::string> see_message = separate_string(parsed_message[0]);
            store_data_see(see_message, player, ball, own_goal, opponent_goal);

            // Logic of the player
            if (player.see_ball)
            {
                switch (player.unum)
                {
                case 1:
                    // Portero
                    if (ball.distance < 1.5)
                    {
                       
                        int power = 100;
                        std::string kick_command = "(kick " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(kick_command, server_udp);
                    }
                    else if (ball.distance < 30)
                    {
                        
                        int power = 100;
                        std::string dash_command = "(dash " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(dash_command, server_udp);
                    }
                    else
                    {
                        
                        std::string rotate_command = "(turn " + std::to_string(30) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    
                    if (ball.distance < 1.5)
                    {
                        // Kick the ball
                        int power = 100;
                        std::string kick_command = "(kick " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(kick_command, server_udp);
                    }
                    else if (ball.distance < 25)
                    {
                        
                        int power = 100;
                        std::string dash_command = "(dash " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(dash_command, server_udp);
                    }
                    else
                    {
                        
                        int division = (ball.distance < 6) ? 20 : 5;
                        std::string rotate_command = "(turn " + std::to_string(ball.angle / division) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }
                    break;
                case 6:
                case 8:
                case 10:
                    // Centrocampistas
                    if (ball.distance < 1.5)
                    {
                        
                        int power = 100;
                        std::string kick_command = "(kick " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(kick_command, server_udp);
                    }
                    else if (ball.distance < 25)
                    {
                      
                        int power = (ball.distance < 3) ? 60 : ((ball.distance < 7) ? 80 : 100);
                        std::string dash_command = "(dash " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(dash_command, server_udp);
                    }
                    else
                    {
                       
                        int division = (ball.distance < 6) ? 20 : 5;
                        std::string rotate_command = "(turn " + std::to_string(ball.angle / division) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }
                    break;
                case 7:
                case 9:
                case 11:
                    // Delanteros y extremos
                    if (ball.distance < 1.5)
                    {
                        // Kick the ball
                        int power = 100;
                        std::string kick_command = "(kick " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(kick_command, server_udp);
                    }
                    else if (ball.distance < 30)
                    {
                        // Dash towards the ball
                        int power = (ball.distance < 3) ? 60 : ((ball.distance < 7) ? 80 : 100);
                        std::string dash_command = "(dash " + std::to_string(power) + " 0)";
                        udp_socket.sendTo(dash_command, server_udp);
                    }
                    else
                    {
                        // Rotate to face the ball
                        int division = (ball.distance < 6) ? 20 : 5;
                        std::string rotate_command = "(turn " + std::to_string(ball.angle / division) + ")";
                        udp_socket.sendTo(rotate_command, server_udp);
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                std::cout << "Ball not seen" << std::endl;
                // Rotate to find the ball
                std::string rotate_command = (player.y < 0) ? "(turn -80)" : "(turn 80)";
                udp_socket.sendTo(rotate_command, server_udp);
            }
        }
    }

    return 0;
}
