#include "ui.hpp"
#include "util.hpp"


Button::Button(const std::shared_ptr<ImageFont>& font) :
	selected(false),
	font(font) {

	this->text.setFont(font);
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

void Button::activate() const {
	this->callback();
}

void Button::setText(const std::string& str) {
	this->text.setText(str);
	// Place the text somewhat down, and more to the right...
	this->text.setPosition({ this->text.getPosition().x + 5, this->text.getPosition().y + 2});
}

void Button::update(const sf::Time& dt) {
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();

	sf::Color color = sf::Color::Red;
	if (this->isSelected()) {
		color = sf::Color::Green;
	}

	sf::RectangleShape rect;
	rect.setSize({ 200, 25 });
	rect.setOutlineThickness(1);
	rect.setFillColor(sf::Color::Black);
	rect.setOutlineColor(color);

	target.draw(rect, states);
	target.draw(this->text, states);
}

// =============================================================================

 Container::Container() :
	selectedIndex(0) {
 }

 void Container::addButton(const std::shared_ptr<Button>& btn) {
	this->vecButtons.push_back(btn);

	// regardless of the buttons, always select the first one for now.
	this->vecButtons[0]->setSelected(true);

 }

void Container::activate() const {
	int idx = this->selectedIndex % this->vecButtons.size();
	TRACE("Activating button index " << idx);
	this->vecButtons[idx]->activate();
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
		 btn->setSelected(false);
	}

	int idx = btnIndex % this->vecButtons.size();
	TRACE("Selecting button index " << idx);
	this->vecButtons[idx]->setSelected(true);
}

void Container::update(const sf::Time& delta) {
	for (const auto& btn : this->vecButtons) {
		btn->update(delta);
	}
}

 void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	 for (const auto& btn : this->vecButtons) {
		 btn->draw(target, states);
	 }
 }
