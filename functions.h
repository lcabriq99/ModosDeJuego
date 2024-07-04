#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "estructuras.h"
#include <MinimalSocket/udp/UdpSocket.h>
using namespace std;

Player parseInitialMessage(std::string &message, Player &player);
vector<string> separate_string(string &s);
vector<string> separate_string_separator(string &s, string separator);
void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field);

// Función para enviar comandos de movimiento inicial
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient);

// Decisión para las acciones del jugador
void decisionTree(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para chutar a portería
void chutarPorteria(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para calcular la distancia del balón al propio jugador
float calcularDistanciaJugadorBalon(const Player &jugador, const Ball &balon);

// Función para obtener el equipo, dorsal y distancia de un jugador respecto a los que puede ver
void procesarJugadoresVisibles(vector<string> see_message, vector<JugadorCercano> &jugadores_visibles);

// Función para mostrar los jugadores que ve
void mostrarJugadoresVisibles(const vector<JugadorCercano> &jugadores_visibles);


//A PARTIR DE AQUÍ

// Función para procesar mensajes 'hear'
void store_data_hear(string &hear_message, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para manejar modos de juego
void handle_game_mode(const string &modo, Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para manejar saques de portería
void execute_goal_kick(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para manejar saques de banda
void execute_throw_in(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para manejar saques de esquina
void execute_corner_kick(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

// Función para manejar faltas
void execute_free_kick(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

#endif
