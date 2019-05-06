#include "ui.hpp"
#include "util.hpp"

Button::Button(const std::shared_ptr<ImageFont>& font) :
	selected(false),
	font(font) {

	this->text.setFont(font);
	this->text.setKerning(2);
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
	this->time += dt;

	if (this->isSelected()) {
		this->gfx.thickness = 2 * std::sin(this->time.asSeconds() * 2 * M_PI) + 4;
		this->gfx.selectedColor = sf::Color::Green;
	} else {
		this->gfx.thickness = 1;
		this->gfx.selectedColor = sf::Color::Red;
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();

	sf::VertexArray varr;
	varr.setPrimitiveType(sf::PrimitiveType::Quads);

	sf::Vertex v1;
	v1.position.x = 0;
	v1.position.y = 0;
	v1.color = this->gfx.selectedColor;

	sf::Vertex v2;
	v2.position.x = this->width;
	v2.position.y = 0;
	v2.color = this->gfx.selectedColor;

	sf::Vertex v3;
	v3.position.x = this->width;
	v3.position.y = this->height;
	v3.color = sf::Color::Black;

	sf::Vertex v4;
	v4.position.x = 0;
	v4.position.y = this->height;
	v4.color = sf::Color::Black;

	varr.append(v1);
	varr.append(v2);
	varr.append(v3);
	varr.append(v4);

	sf::RectangleShape rect;
	rect.setSize({ this->width, this->height });
	rect.setOutlineThickness(this->gfx.thickness);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(this->gfx.selectedColor);

	target.draw(varr, states);
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
