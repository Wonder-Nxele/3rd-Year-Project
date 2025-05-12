#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"

class MessageBox {
public:
    // Only needs the message and a font (we hard-code "OK").
    // boxSize defaults to 400×150 if you like.
    MessageBox();
    MessageBox(const std::string& message, sf::Font& font, const sf::Vector2f& boxSize = { 400.f,150.f });

    // Position the top-left corner of the dialog
    void setPosition(const sf::Vector2f& pos);

    // Show the dialog
    void open();

    // Consume events; clicking OK will close it.
    void handleEvent(sf::RenderWindow& window, const sf::Event& evt);

    // Draw overlay + box + text + OK button
    void drawTo(sf::RenderWindow& window);

    bool isOpen() const { return m_open; }

private:
    bool              m_open = false;

    sf::RectangleShape m_overlay;
    sf::RectangleShape m_box;
    sf::Text           m_message;

    Button             m_okButton;
};