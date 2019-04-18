#include "ui.hpp"

Button::Button() {

}

Button::Button(const std::shared_ptr<ImageFont> font) :
    font(font) {
}

bool Button::isSelected() const {
    return this->selected;
}

void Button::setSelected(bool selected) {
    this->selected = selected;
}

void Button::setCallback(std::function<void()> callback) {
    this->callback = callback;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= this->getTransform();

    sf::Color color = sf::Color::Red;
    if (this->isSelected()) {
        color = sf::Color::Green;
    }

    sf::RectangleShape rect;
    rect.setSize({ 300, 20 });
    rect.setOutlineThickness(2);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(color);

    target.draw(rect, states);
}

// =============================================================================

 Container::Container() {
 }

 void Container::addButton(const Button& b) {
     this->vecButtons.push_back(b);
 }

void Container::selectPrevious() {
    this->selectedIndex--;

    for (auto& btn : this->vecButtons) {
         btn.setSelected(false);
     }

     this->vecButtons[this->selectedIndex % this->vecButtons.size()].setSelected(true);
}

void Container::selectNext() {
    this->selectedIndex++;

    for (auto& btn : this->vecButtons) {
         btn.setSelected(false);
     }

     this->vecButtons[this->selectedIndex % this->vecButtons.size()].setSelected(true);
}

 void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
     for (const auto& btn : this->vecButtons) {
         btn.draw(target, states);
     }
 }
