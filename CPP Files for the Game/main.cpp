# include <SFML\Graphics.hpp>
# include <string>
#include "Level.h"
# include "Game.h"

using namespace sf;

//Text message(const std::string& my_message, const Font& font, const double& x, const double& y);

template<typename T>
bool loadFile(T& type_of_file, const std::string& file_name);



int main() {
	Game* game = new Game();
	//Game game;
	game->run();
	return 0;
}




