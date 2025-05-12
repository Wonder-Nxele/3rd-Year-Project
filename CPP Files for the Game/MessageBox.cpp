#include "MessageBox.h"

MessageBox::MessageBox()
{
}

MessageBox::MessageBox(const std::string& message, sf::Font& font, const sf::Vector2f& boxSize) : m_okButton("Okay", { 80,30 }, 18, font, sf::Color::Red, sf::Color::White)
{
    // semi-transparent overlay
    m_overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // dialog box
    m_box.setSize(boxSize);
    m_box.setFillColor(sf::Color::White);
    m_box.setOutlineThickness(2);
    m_box.setOutlineColor(sf::Color::Blue);

    // message text
    m_message.setFont(font);
    m_message.setString(message);
    m_message.setCharacterSize(18);
    m_message.setFillColor(sf::Color::Black);
}

void MessageBox::setPosition(const sf::Vector2f& pos) {
    // dialog box
    m_box.setPosition(pos);

    // message text with 10px margin
    m_message.setPosition(pos + sf::Vector2f(10, 10));

    // OK button centered at bottom
    float bx = pos.x + (m_box.getSize().x - m_okButton.button.getSize().x) / 2;
    float by = pos.y + m_box.getSize().y - m_okButton.button.getSize().y - 10;
    m_okButton.setPosition({ bx, by });
}

void MessageBox::open() {
    // cover entire desktop
  //  auto vm = sf::VideoMode::getDesktopMode();
   // m_overlay.setSize({ float(vm.width), float(vm.height) });
    m_open = true;
}

void MessageBox::handleEvent(sf::RenderWindow& window, const sf::Event& evt) {
    if (!m_open) return;

    if (evt.type == sf::Event::MouseButtonPressed &&
        evt.mouseButton.button == sf::Mouse::Left)
    {
        // reuse your Button::handleClick logic
        auto res = m_okButton.handleClick(window, evt);
        if (!res.isEmpty()) {
            m_open = false;
        }
    }
}

void MessageBox::drawTo(sf::RenderWindow& window) {
    if (!m_open) return;

    window.draw(m_overlay);
    window.draw(m_box);
    window.draw(m_message);
    m_okButton.drawTo(window);
}