#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <MinimalSocket/udp/UdpSocket.h>
#include "estructuras.h"

// Function declarations
Player parseInitialMessage(std::string &message, Player &player);
std::vector<std::string> separate_string(std::string &s);
std::vector<std::string> separate_string_separator(std::string &s, std::string separator);
void store_data_see(std::vector<std::string> &see_message, Player &player, Ball &ball, CentroPorteriaDerecha &goal_R, CentroPorteriaIzquierda &goal_L);
void handlePlayerActions(Player &player, Ball &ball, CentroPorteriaDerecha &goal_R, CentroPorteriaIzquierda &goal_L, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp);
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient);
