#include "ui.hpp"
#include "util.hpp"

Button::Button() :
	selected(false) {

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

 Container::Container() :
	selectedIndex(0) {
 }

 void Container::addButton(const Button& b) {
	this->vecButtons.push_back(b);

	// regardless of the buttons, always select the first one for now.
	this->vecButtons[0].setSelected(true);
 }

void Container::selectPrevious() {
	this->selectedIndex--;
	// Wrap around if the selectedIndex goes negative
	if (this->selectedIndex < 0) {
		this->selectedIndex = this->vecButtons.size() - 1;
	}
	this->select(this->selectedIndex);
}

void Container::selectNext() {
	this->selectedIndex++;
	this->select(this->selectedIndex);
}

void Container::select(int btnIndex) {
	// First just deselect everything before marking the btnIndex as selected.
	for (auto& btn : this->vecButtons) {
		 btn.setSelected(false);
	}

	int idx = btnIndex % this->vecButtons.size();
	TRACE("Selecting button index " << idx);
	this->vecButtons[idx].setSelected(true);
}

 void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	 for (const auto& btn : this->vecButtons) {
		 btn.draw(target, states);
	 }
 }
