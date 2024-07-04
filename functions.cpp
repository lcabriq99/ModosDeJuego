#include "functions.h"
#include <iostream>
#include <sstream>

Player parseInitialMessage(std::string &message, Player &player)
{
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = message.find(delimiter)) != std::string::npos)
    {
        token = message.substr(0, pos);
        if (i == 1)
        {
            player.side = token;
        }
        else if (i == 2)
        {
            player.unum = std::stoi(token);
        }
        else if (i == 3)
        {
            player.playmode = token;
        }
        message.erase(0, pos + delimiter.length());
        i++;
    }
    cout << "Player: " << player.side << " " << player.unum << " " << player.playmode << endl;
    return player;
}

vector<string> separate_string_separator(string &s, string separator)
{
    vector<string> v;
    int pos = 0;
    while ((pos = s.find(separator)) != string::npos)
    {
        v.push_back(s.substr(0, pos));
        s.erase(0, pos + separator.length());
    }
    v.push_back(s);
    return v;
}

vector<string> separate_string(string &s)
{
    vector<string> v;
    int pos{0};
    int level{0};
    string temp;

    while (pos < s.size())
    {
        if (s[pos] == '(')
        {
            level++;
        }
        else if (s[pos] == ')')
        {
            level--;
        }

        if (s[pos] == '(' && level == 1)
        {
            temp = "";
        }
        else if (s[pos] == ')' && level == 0)
        {
            v.push_back(temp);
        }
        else
        {
            temp += s[pos];
        }
        pos++;
    }

    if (level != 0)
    {
        throw std::runtime_error("No se han abierto y cerrado los mismos parentesis");
    }
    else
    {
        return v;
    }
}

void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field)
{
    vector<string> ball_coords;
    bool found_ball = false;
    player.flags_seen = 0;
    player.see_ball = false;
    player.see_own_goal = false;
    player.see_opponent_goal = false;

    for (size_t i = 0; i < see_message.size(); i++)
    {
        if (see_message[i].find("(b)") != string::npos)
        {
            player.see_ball = true;
            ball_coords = separate_string_separator(see_message[i], " ");
            ball.x = ball_coords[1];
            ball.y = ball_coords[2];
            double distance = sqrt(pow(stod(ball.x), 2) + pow(stod(ball.y), 2));
            ball.distance = distance;
            double angle = atan2(stod(ball.y), stod(ball.x));
            ball.angle = angle * 180 / M_PI;
            found_ball = true;
        }

        if (see_message[i].find("(g r)") != string::npos)
        {
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "r")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                own_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                own_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                opponent_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                player.see_opponent_goal = true;
            }
        }

        if (see_message[i].find("(g l)") != string::npos)
        {
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "l")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                own_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                own_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                opponent_goal.distance = sqrt(pow(stof(goal_coords[2]), 2) + pow(stof(goal_coords[3]), 2));
                player.see_opponent_goal = true;
            }
        }

        if (see_message[i].find("(f c)") != string::npos)
        {
            vector<string> center_coords = separate_string_separator(see_message[i], " ");
            field.flag_center = {stof(center_coords[2]), stof(center_coords[3])};
            field.flag_center_distance = sqrt(pow(stof(center_coords[2]), 2) + pow(stof(center_coords[3]), 2));
        }

        if (see_message[i].find("(f c t)") != string::npos)
        {
            vector<string> center_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_top = {stof(center_top_coords[3]), stof(center_top_coords[4])};
            field.flag_center_top_distance = sqrt(pow(stof(center_top_coords[3]), 2) + pow(stof(center_top_coords[4]), 2));
            player.flags_seen++;
        }

        if (see_message[i].find("(f c b)") != string::npos)
        {
            vector<string> center_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_bottom = {stof(center_bottom_coords[3]), stof(center_bottom_coords[4])};
            field.flag_center_bottom_distance = sqrt(pow(stof(center_bottom_coords[3]), 2) + pow(stof(center_bottom_coords[4]), 2));
            player.flags_seen++;
        }

        if (see_message[i].find("(f l t)") != string::npos)
        {
            vector<string> left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_top = {stof(left_top_coords[3]), stof(left_top_coords[4])};
            field.flag_left_top_distance = sqrt(pow(stof(left_top_coords[3]), 2) + pow(stof(left_top_coords[4]), 2));
            field.flag_left_top_angle = atan2(stof(left_top_coords[4]), stof(left_top_coords[3])) * 180 / M_PI;
            player.flags_seen++;
            player.see_left_top = true;
        }

        if (see_message[i].find("(f l b)") != string::npos)
        {
            vector<string> left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_bottom = {stof(left_bottom_coords[3]), stof(left_bottom_coords[4])};
            field.flag_left_bottom_distance = sqrt(pow(stof(left_bottom_coords[3]), 2) + pow(stof(left_bottom_coords[4]), 2));
            player.flags_seen++;
        }

        if (see_message[i].find("(f r b)") != string::npos)
        {
            vector<string> right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_bottom = {stof(right_bottom_coords[3]), stof(right_bottom_coords[4])};
            field.flag_right_bottom_distance = sqrt(pow(stof(right_bottom_coords[3]), 2) + pow(stof(right_bottom_coords[4]), 2));
            player.flags_seen++;
        }
    }
    if (found_ball == false)
    {
        player.see_ball = false;
    }
}

void chutarPorteria(Player &player, Ball &ball, Goal &opponent_goal, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    float angle = opponent_goal.angle; // Ángulo hacia la portería contraria
    int power = 100; // Potencia del chute
    std::string kick_command = "(kick " + to_string(power) + " " + to_string(angle) + ")";
    udp_socket.sendTo(kick_command, server_udp); // Enviar comando de chute
}

float calcularDistanciaJugadorBalon(const Player &jugador, const Ball &balon)
{
    return sqrt(pow(stof(balon.x) - jugador.x, 2) + pow(stof(balon.y) - jugador.y, 2));
}

void procesarJugadoresVisibles(vector<string> see_message, vector<JugadorCercano> &jugadores_visibles)
{
    jugadores_visibles.clear();
    for (auto &obj : see_message)
    {
        if (obj.find("(p") != string::npos && obj.find("(p)") == string::npos)
        {
            vector<string> player_info = separate_string_separator(obj, " ");
            JugadorCercano jugador;
            jugador.nombreEquipo = player_info[1];
            jugador.dorsal = player_info[2];
            jugador.distancia = player_info[3];

            if (!jugador.dorsal.empty() && jugador.dorsal.back() == ')')
            {
                jugador.dorsal.pop_back();
            }

            jugadores_visibles.push_back(jugador);
        }
    }
}

void mostrarJugadoresVisibles(const vector<JugadorCercano> &jugadores_visibles)
{
    std::cout << "Jugadores visibles: " << std::endl;
    for (const auto &jugador : jugadores_visibles)
    {
        std::cout << "Nombre Equipo: " << jugador.nombreEquipo << std::endl;
        std::cout << "Dorsal: " << jugador.dorsal << std::endl;
        std::cout << "Distancia: " << jugador.distancia << std::endl;
    }
}


//APARTIR DE AQUÍ

void store_data_hear(string &hear_message, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    vector<string> aux_hear_message = separate_string(hear_message); // hear 0 referee kick_off_l
    vector<string> valores_mensaje_Hear;

    for (size_t i = 0; i < aux_hear_message.size(); i++)
    {
        if (aux_hear_message[i].find("hear") != string::npos)
        {
            valores_mensaje_Hear = separate_string_separator(aux_hear_message[i], " ");
            if (valores_mensaje_Hear.size() >= 4)
            {
                cout << "TIME: " << valores_mensaje_Hear[1] << endl;
                cout << "REFEREE: " << valores_mensaje_Hear[2] << endl;
                cout << "MODO: " << valores_mensaje_Hear[3] << endl;

                string modo = valores_mensaje_Hear[3];
                handle_game_mode(modo, udp_socket, server_udp);
            }
            else
            {
                cout << "Error: mensaje 'hear' no tiene suficientes elementos" << endl;
            }
        }
    }
}

//Para que lo entendais el numero de delante 1 o 2 es el jugador que quiero que ejecute la acción , saque de puerta solo el 1(portero)
//Saque de banda o corner solo el 2 porque solo pruebo con 2 jugadores asi que se lo asigno al 2
//Pero no me hace ni puto caso
void handle_game_mode(const string &modo, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    if (modo == "kick_off_l" || modo == "kick_off_r")
    {
        cout << "Saque inicial: " << modo << endl;
    }
    else if (modo == "free_kick_l" || modo == "free_kick_r")
    {
        cout << "Tiro libre: " << modo << endl;
        execute_free_kick(1, udp_socket, server_udp); // Ejemplo de ejecución de falta
    }
    else if (modo == "goal_kick_l" || modo == "goal_kick_r")
    {
        cout << "Saque de puerta: " << modo << endl;
        execute_goal_kick(1, udp_socket, server_udp); // Portero (jugador 1)
    }
    else if (modo == "corner_kick_l" || modo == "corner_kick_r")
    {
        cout << "Saque de esquina: " << modo << endl;
        execute_corner_kick(2, udp_socket, server_udp); // Jugador 2
    }
    else if (modo == "throw_in_l" || modo == "throw_in_r")
    {
        cout << "Saque de banda: " << modo << endl;
        execute_throw_in(2, udp_socket, server_udp); // Jugador 2
    }
    else if (modo == "offside_l" || modo == "offside_r")
    {
        cout << "Fuera de juego: " << modo << endl;
    }
    else if (modo == "goal_l" || modo == "goal_r")
    {
        cout << "Gol: " << modo << endl;
    }
    else if (modo == "play_on")
    {
        cout << "Juego en curso" << endl;
    }
    else if (modo == "time_over")
    {
        cout << "Fin del tiempo de juego" << endl;
    }
    else
    {
        cout << "Modo desconocido: " << modo << endl;
    }
}

void execute_goal_kick(int player_id, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    std::string command = "(kick 100 0)";
    udp_socket.sendTo(command, server_udp);
    cout << "Jugador " << player_id << " ejecuta saque de puerta con potencia 100" << endl;
}

void execute_throw_in(int player_id, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    std::string command = "(kick 100 -90)"; // Potencia 100 y dirección hacia dentro del campo
    udp_socket.sendTo(command, server_udp);
    cout << "Jugador " << player_id << " ejecuta saque de banda con potencia 100 y dirección hacia dentro del campo" << endl;
}

void execute_corner_kick(int player_id, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    std::string command = "(kick 100 45)"; // Potencia 100 y dirección adecuada
    udp_socket.sendTo(command, server_udp);
    cout << "Jugador " << player_id << " ejecuta saque de esquina con potencia 100 y dirección hacia dentro del campo" << endl;
}

void execute_free_kick(int player_id, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &server_udp)
{
    std::string command = "(kick 100 0)";
    udp_socket.sendTo(command, server_udp);
    cout << "Jugador " << player_id << " ejecuta falta con potencia 100" << endl;
}
