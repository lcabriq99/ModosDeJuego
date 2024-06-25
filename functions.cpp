#include "functions.h"

// Parse initial message function (init Side Unum PlayMode)
Player parseInitialMessage(std::string &message, Player &player) {
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = message.find(delimiter)) != std::string::npos) {
        token = message.substr(0, pos);
        if (i == 1) {
            player.side = token;
        } else if (i == 2) {
            player.unum = std::stoi(token);
        } else if (i == 3) {
            player.playmode = token;
        }
        message.erase(0, pos + delimiter.length());
        i++;
    }
    std::cout << "Player: " << player.side << " " << player.unum << " " << player.playmode << std::endl;
    return player;
}

// Separate string function using separator
std::vector<std::string> separate_string_separator(std::string &s, std::string separator) {
    std::vector<std::string> v;
    int pos = 0;
    while ((pos = s.find(separator)) != std::string::npos) {
        v.push_back(s.substr(0, pos));
        s.erase(0, pos + separator.length());
    }
    v.push_back(s);
    return v;
}

// Separate string function for parsing
std::vector<std::string> separate_string(std::string &s) {
    std::vector<std::string> v;
    int pos = 0;
    int level = 0;
    std::string temp;

    while (pos < s.size()) {
        if (s[pos] == '(') {
            level++;
        } else if (s[pos] == ')') {
            level--;
        }

        if (s[pos] == '(' && level == 1) {
            temp = "";
        } else if (s[pos] == ')' && level == 0) {
            v.push_back(temp);
        } else {
            temp += s[pos];
        }
        pos++;
    }

    if (level != 0) {
        throw std::runtime_error("No se han abierto y cerrado los mismos parentesis");
    } else {
        return v;
    }
}

// Store data from 'see' message in player and objects
void store_data_see(std::vector<std::string> &see_message, Player &player, Ball &ball, CentroPorteriaDerecha &goal_R, CentroPorteriaIzquierda &goal_L) {
    bool found = false;
    for (size_t i = 0; i < see_message.size(); i++) {
        if (see_message[i].find("(b)") != std::string::npos) {
            std::cout << "The player sees the ball" << std::endl;
            player.see_ball = true;
            std::vector<std::string> ball_coords = separate_string_separator(see_message[i], " ");
            ball.x = ball_coords[1];
            ball.y = ball_coords[2];
            std::cout << "Ball coordinates: " << ball.x << " " << ball.y << std::endl;
            double distance = sqrt(std::pow(std::stod(ball.x), 2) + std::pow(std::stod(ball.y), 2));
            ball.distance = distance;
            double angle = atan2(std::stod(ball.y), std::stod(ball.x)) * 180 / M_PI;
            ball.angle = angle;
            found = true;
        }

        if (see_message[i].find("(g r)") != std::string::npos) {
            player.see_goal = true;
            std::vector<std::string> right_goal_coords = separate_string_separator(see_message[i], " ");
            goal_R.x = right_goal_coords[2];
            goal_R.y = right_goal_coords[3];
        }

        if (see_message[i].find("(g l)") != std::string::npos) {
            player.see_goal = true;
            std::vector<std::string> left_goal_coords = separate_string_separator(see_message[i], " ");
            goal_L.x = left_goal_coords[2];
            goal_L.y = left_goal_coords[3];
        }
    }
    if (!found) {
        player.see_ball = false;
    }
}

// Handle player actions based on current state
void handlePlayerActions(Player &player, Ball &ball, CentroPorteriaDerecha &goal_R, CentroPorteriaIzquierda &goal_L, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp) {
    if (player.see_ball) {
        if (ball.distance < 1.5) {
            int power = 100;
            std::string kick_command = "(kick " + std::to_string(power) + " 0)";
            udp_socket.sendTo(kick_command, server_udp);
            std::cout << "Kick command sent: " << kick_command << std::endl;
        } else {
            int division = (ball.distance < 6) ? 20 : 5;
            std::string rotate_command = "(turn " + std::to_string(ball.angle / division) + ")";
            udp_socket.sendTo(rotate_command, server_udp);
        }
    } else {
        std::string rotate_command = (player.y < 0) ? "(turn -80)" : "(turn 80)";
        udp_socket.sendTo(rotate_command, server_udp);
    }
}

// Send initial move message to server
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient) {
    static std::vector<std::pair<int, int>> positions = {
        {-50, 0}, {-40, -10}, {-35, -28}, {-40, 10}, {-35, 28}, 
        {-25, 11}, {-8, 20}, {-25, -11}, {-5, 0}, {-15, 0}, {-8, -20}
    };
  
    auto myPos = positions[player.unum - 1];

    auto moveCommand = "(move " + std::to_string(myPos.first) + " " + std::to_string(myPos.second) + ")";
    udp_socket.sendTo(moveCommand, recipient);
    std::cout << "Move command sent: " << moveCommand << std::endl;
}
