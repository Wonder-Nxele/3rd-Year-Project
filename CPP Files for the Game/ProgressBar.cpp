#include "ProgressBar.h"

ProgressBar::ProgressBar(sf::Vector2f position, sf::Vector2f size) : maxWidth_(size.x)
{
    // Track background (dark gray)
    track_.setPosition(position);
    track_.setSize(size);
    track_.setFillColor(sf::Color(50, 50, 50));
    track_.setOutlineColor(sf::Color(100, 100, 100));
    track_.setOutlineThickness(2.f);

    // Progress fill (green)
    fill_.setPosition(position);
    fill_.setSize({ 0.f, size.y });
    fill_.setFillColor(sf::Color(100, 250, 50));
}

void ProgressBar::setProgress(float percent) {
    if (percent < 0.f) percent = 0.f;
    if (percent > 1.f) percent = 1.f;

    fill_.setSize({ maxWidth_ * percent, fill_.getSize().y });
}

void ProgressBar::draw(sf::RenderWindow& window) const {
    window.draw(track_);
    window.draw(fill_);
}

void ProgressBar::setFillColor(const sf::Color& color) {
    fill_.setFillColor(color);
}