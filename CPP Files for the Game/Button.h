#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
	sf::RectangleShape button;
	sf::Text text;
	Button();
	Button(std::string t, sf::Vector2f size, int charSize, sf::Font& font, sf::Color bgColor, sf::Color textColor);
	void setBackColor(sf::Color color);
	void setTextColor(sf::Color color);
	void setPosition(sf::Vector2f pos);

	void drawTo(sf::RenderWindow& window);
	bool isMouseOver(sf::RenderWindow& window);
	bool isButtonClicked(sf::RenderWindow& window);


	bool clicked = false;	
	sf::String handleClick(sf::RenderWindow& window, sf::Event event);
	inline bool isClicked() const
	{
		return clicked;
	}
	inline void unclick() { clicked = false; }
private:


};
