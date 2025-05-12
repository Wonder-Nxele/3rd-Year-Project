#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

// forward declare the QuestionBank class
class QuestionBank;

class Player
{
private:
	string username;		   // user name of the player
	int currentLvl = 0;			   // level the player is currently on
	int hints;				   //  hints of player
	const int MAX_HINTS = 5;   // maximum hints
public:
	Player() {} // just for now
	Player(string username);   // constructor will receive name the other attributes are set to zero
	string getName() const { return this->username; }
	void setUsername(string name) { this->username = name; }
	int& getLevel() { return currentLvl; }

	// declare a QuestionBank as a friend
	friend class QuestionBank;

	// overloading operator
	Player& operator++();
};
