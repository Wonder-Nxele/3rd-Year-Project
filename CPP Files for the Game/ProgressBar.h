#pragma once
#include <SFML/Graphics.hpp>

class ProgressBar {
private:

public:
    sf::RectangleShape track_;
    sf::RectangleShape fill_;
    float maxWidth_;

    ProgressBar(sf::Vector2f position, sf::Vector2f size);  // ✅ MATCH THIS
    void setProgress(float percent);
    void draw(sf::RenderWindow& window) const;
    void setFillColor(const sf::Color& color);
    float progressPercent = 0.f;   //Incremented whenever you select an answer   

};