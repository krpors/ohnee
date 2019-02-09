#include <memory>
#include <iostream>
#include <sstream>

#include <cmath>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "text.hpp"

BobbingText::BobbingText(const std::shared_ptr<ImageFont>& font) {
	this->font = font;
	this->timer = 0.0f;
	this->text.setFont(this->font);
	std::stringstream ss;
	ss << font->getCharacters() << std::endl;
	ss << "Derp de derp.";
	this->text.setText(20, 20, ss.str());


}

BobbingText::~BobbingText() {
}

void BobbingText::update(const sf::Time& dt) {
	this->timer += dt.asSeconds();
}

void BobbingText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	float f = 1.2f * fabs(sin(3.14f * this->timer)) + 1.0f;

	std::cout << f << std::endl;
	sf::Transform t;
	t.transformPoint(100, 200);
	t.scale(f, f);
	states.transform *= t;
	target.draw(this->text, states);
}