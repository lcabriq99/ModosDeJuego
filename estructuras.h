// estructuras.h
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include<iostream>
#include <vector>


//A PARTIR DE AQUÍ
struct Player {
    std::string team_name;
    std::string playmode;
    std::string side;
    bool see_ball;
    bool see_own_goal;
    bool see_opponent_goal;
    bool see_left_top;
    int flags_seen;
    int unum;
    double x;
    double y;
    double angle; 
    Player() : see_ball(false), see_own_goal(false), see_opponent_goal(false), see_left_top(false), flags_seen(0), unum(0), x(0), y(0), angle(0) {}
};


struct Ball
{
    std::string x;
    std::string y;
    std::string vx;
    std::string vy;
    double distance;
    double angle;
    Ball() : distance(0), angle(0) {}
};

struct Goal
{
    std::string x;
    std::string y;
    std::string side;
    float distance;
    double angle;
    Goal() : distance(0), angle(0) {}
};

//Violeta
struct JugadorCercano
{
    std::string x;
    std::string y;
    std::string distancia;
    std::string unum;
    std::string nombreEquipo;
    std::string dorsal;
    float distance;
};

struct JugadoresAlrededor
{
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> distancia;
    std::vector<int> unum;
    float distance;
};

struct Field
{
    std::vector<float> flag_center;
    float flag_center_distance;
    std::vector<float> flag_center_top;
    float flag_center_top_distance;
    std::vector<float> flag_center_bottom;
    float flag_center_bottom_distance;
    std::vector<float> flag_left_top;
    float flag_left_top_distance;
    float flag_left_top_angle;
    std::vector<float> flag_left_bottom;
    float flag_left_bottom_distance;
    std::vector<float> flag_right_top;
    float flag_right_top_distance;
    std::vector<float> flag_right_bottom;
    float flag_right_bottom_distance;
    Field() : flag_center_distance(0), flag_center_top_distance(0), flag_center_bottom_distance(0),
              flag_left_top_distance(0), flag_left_top_angle(0), flag_left_bottom_distance(0),
              flag_right_top_distance(0), flag_right_bottom_distance(0) {}
};

#endif // ESTRUCTURAS_H











