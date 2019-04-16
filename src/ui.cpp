#include "ui.hpp"

Button::Button() {

}

Button::Button(const std::shared_ptr<ImageFont> font) :
    font(font) {
}

void Button::setCallback(std::function<void()> callback) {
    this->callback = callback;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= this->getTransform();

    sf::RectangleShape rect;
    rect.setSize({ 300, 20 });
    rect.setOutlineThickness(2);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);

    target.draw(rect, states);
}
