/**
 * @file Player.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "Player.h"
using namespace server;
using namespace std;

Player::Player(string pseudo, bool isLogged, int teamId)
    : pseudo(pseudo), isLogged(isLogged), teamId(teamId) {}
