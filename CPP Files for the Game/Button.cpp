#include "Button.h"
#include "Level.h"
#include <iostream>
Button::Button()
{
}
//Constructor for buttons class
Button::Button(std::string t, sf::Vector2f size, int charSize, sf::Font& font, sf::Color bgColor, sf::Color textColor)
{
	text.setFont(font);
	text.setString(t);
	text.setFillColor(textColor);
	text.setCharacterSize(charSize);

	button.setSize(size);
	button.setFillColor(bgColor);
}

//Added method to help wit event processing
bool Button::isButtonClicked(sf::RenderWindow& window) {
	// Check if the left mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


		// Convert mouse position to world coordinates(so that when the window is expanded the function can also work)
		// Convert the current mouse position from pixel coordinates to world coordinates,
		// so it stays accurate even when the window is resized or zoomed
		sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		// Extract the x and y coordinates of the mouse in world space
		float mouseX = worldPos.x;
		float mouseY = worldPos.y;

		// Get the button's top-left corner position
		float btnPosX = button.getPosition().x;
		float btnPosY = button.getPosition().y;

		// Calculate the bottom-right corner of the button
		float btnxPosWidth = btnPosX + button.getLocalBounds().width;     // Gets the width of the button
		float btnyPosHeight = btnPosY + button.getLocalBounds().height;   // Gets the height of the button

		// Check if the mouse click is within the button's boundaries
		if (mouseX < btnxPosWidth && mouseX > btnPosX &&
			mouseY < btnyPosHeight && mouseY > btnPosY) {
			return true;// The button was clicked
		}
	}
	return false;
}


void Button::setBackColor(sf::Color color)
{
	button.setFillColor(color);
}

void Button::setTextColor(sf::Color color)
{
	text.setFillColor(color);
}

void Button::setPosition(sf::Vector2f pos)
{
	button.setPosition(pos);
	float xPos = (pos.x + button.getLocalBounds().width / 2.5) - (text.getLocalBounds().width / 2.5);
	float yPos = (pos.y + button.getLocalBounds().height / 4) - (text.getLocalBounds().height / 4);
	text.setPosition(xPos, yPos);
}

void Button::drawTo(sf::RenderWindow& window)
{
	window.draw(button);
	window.draw(text);
}

bool Button::isMouseOver(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	float mouseX = mousePos.x;
	float mouseY = mousePos.y;

	float btnPosX = button.getPosition().x;
	float btnPosY = button.getPosition().y;

	float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
	float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

	if (mouseX<btnxPosWidth && mouseX>btnPosX && mouseY<btnyPosHeight && btnyPosHeight && mouseY >btnPosY) {
		return true;
	}
	return false;
}
sf::String Button::handleClick(sf::RenderWindow& window, sf::Event event)
{
	if (isMouseOver(window))
	{

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left) {
				clicked = true;

				std::cout << "Answer Selected: " << text.getString().toAnsiString() << std::endl;
				setBackColor(sf::Color::Yellow);
				return text.getString();
			}
		}
	}

	setBackColor(sf::Color::Red);
	if (isButtonClicked(window)) {
	

	
	}
	return sf::String();
}