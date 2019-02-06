#include <memory>

#include "bob.hpp"
#include "text.hpp"

BobbingText::BobbingText(const std::shared_ptr<ImageFont>& font) {
	this->font = font;

	this->text.setFont(this->font);
	this->text.setText(20, 20, "HERRO THERE!");
}

BobbingText::~BobbingText() {
}

void BobbingText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform t;
	t.scale(6, 6);
	states.transform *= t;
	target.draw(this->text, states);
	t.translate(20, 20);
	target.draw(this->text, states);
}