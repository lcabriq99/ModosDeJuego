#include <iostream>
using namespace std;
#include <MinimalSocket/udp/UdpSocket.h>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <cmath>
#include "functions.h"
#include "estructuras.h"

// Función para enviar el comando de movimiento inicial
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient)
{
    struct Posicion
    {
        int x;
        int y;
    };

    vector<Posicion>
        posiciones = {{-50, 0},
                      {-40, -10},
                      {-35, -28},
                      {-40, 10},
                      {-35, 28},
                      {-25, 11},
                      {-8, 20},
                      {-25, -11},
                      {-5, 0},
                      {-15, 0},
                      {-8, -20}};

    Posicion myPos = posiciones[player.unum - 1];

    auto moveCommand = "(move " + to_string(myPos.x) + " " + to_string(myPos.y) + ")";
    udp_socket.sendTo(moveCommand, recipient);
    cout << "Move command sent. Posicion: " << moveCommand << endl;
}

// Decisión para las acciones del jugador
void decisionTree(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    if (player.see_ball)
    {
        //Violeta
        // Calcular la distancia al balón respecto al jugador
        float distancia_balon = calcularDistanciaJugadorBalon(player, ball);
        //Violeta
        //Mostrar la distancia del jugador especto al balón
        std::cout << "Distancia al balón: " << distancia_balon << std::endl;

        if (ball.distance < 1)
        {
            //Violeta
            // Para chutar a portería
            chutarPorteria(player, ball, opponent_goal, udp_socket, server_udp);
        }
        else
        {
            if (abs(ball.angle) >= 10)
            {
                // Rotar hacia la dirección del balón
                int division = (ball.distance < 6) ? 20 : 5;
                std::string rotate_command = "(turn " + to_string(ball.angle / division) + ")";
                udp_socket.sendTo(rotate_command, server_udp); // Enviar comando de rotación
            }
            else
            {
                // Mover hacia el balón
                int power = (ball.distance < 3) ? 60 : (ball.distance < 7) ? 80 : 100;
                std::string dash_command = "(dash " + to_string(power) + " 0)";
                udp_socket.sendTo(dash_command, server_udp); // Enviar comando de movimiento
            }
        }
    }
    else
    {
        // Rotar para buscar el balón
        int angle = (player.y < 0) ? -80 : 80;
        std::string rotate_command = "(turn " + to_string(angle) + ")";
        udp_socket.sendTo(rotate_command, server_udp); // Enviar comando de rotación
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <team-name> <this-port>" << endl;
        return 1;
    }

    string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);

    cout << "Creating a UDP socket" << endl;

    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);

    cout << "Socket created" << endl;

    bool success = udp_socket.open();

    if (!success)
    {
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

    Player player;
    player.team_name = team_name;

    Ball ball;
    Goal own_goal;
    Goal opponent_goal;
    Field field;

    //Violeta
    //Vector donde se guardan los jugadores visibles para cada jugador
    vector<JugadorCercano> jugadores_visibles;

    player = parseInitialMessage(received_message_content, player);

    cout << player.unum << endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);

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

    float posicion_anterior_x = player.x;
    float posicion_anterior_y = player.y;
    float posicion_actual_x = player.x;
    float posicion_actual_y = player.y;
    float angulo_anterior = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        vector<string> parsed_message = separate_string(received_message_content);

        if (parsed_message[0].find("see") <= 5)
        {
            vector<string> see_message = separate_string(parsed_message[0]);
            store_data_see(see_message, player, ball, own_goal, opponent_goal, field);
            decisionTree(player, ball, opponent_goal, udp_socket, server_udp);

            //Violeta
            // Procesar jugadores visibles 
            procesarJugadoresVisibles(see_message, jugadores_visibles);
            //Violeta
            // Mostrar jugadores visibles
            mostrarJugadoresVisibles(jugadores_visibles);
        }

        posicion_anterior_x = posicion_actual_x;
        posicion_anterior_y = posicion_anterior_y;
        posicion_actual_x = player.x;
        posicion_actual_y = player.y;
        angulo_anterior = atan2(posicion_actual_y - posicion_anterior_y, posicion_actual_x - posicion_anterior_x) * 180 / M_PI;
    }
    return 0;
}
