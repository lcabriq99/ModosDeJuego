#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <cmath>
#include"estructuras.h"
#include <MinimalSocket/udp/UdpSocket.h>
using namespace std;

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);

void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field);

//Violeta
//Funcion para chutar a portería
void chutarPorteria(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);

//Violeta
//Funcion Calcular distancia del balón al propio Jugador
float calcularDistanciaJugadorBalon(const Player& jugador, const Ball& balon);

//Violeta
//Función para obtener el equipo, dorsal y distancia de un jugador respecto a los que puede ver
void procesarJugadoresVisibles(vector<string> see_message, vector<JugadorCercano> &jugadores_visibles);

//Violeta
//Función para mostrar los jugadores que ve
void mostrarJugadoresVisibles(const vector<JugadorCercano> &jugadores_visibles);



