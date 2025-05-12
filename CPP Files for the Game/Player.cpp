// Ntulikazi and Anele
#include "Player.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
Player::Player(string username) {
	this->username = username;
	this->hints = MAX_HINTS;
}

Player& Player::operator++() {
	++currentLvl;
	return *this;
}