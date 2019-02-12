#include <memory>
#include <iostream>
#include <sstream>

#include <cmath>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "text.hpp"

BobbingText::BobbingText(const std::shared_ptr<ImageFont>& font) :
		Text(font) {

	this->timer = 0.0f;
	std::stringstream ss;
	ss << "I am currently watching" << std::endl
	   << "the Tragedy of systemd." << std::endl
	   << "!@#$%^&*()-=_+|/\\:;'\"<>,.?" << std::endl;

	Text::setText(150, 300, ss.str());
	Text::setOrigin(250, 300 + (3 * 24 / 2));
}

BobbingText::~BobbingText() {

}

void BobbingText::update(const sf::Time& dt) {
	this->timer += dt.asSeconds();
	float f = 1.2f * fabs(sin(3.14f * this->timer)) + 1.0f;
	float r = 20.0f * cos(3.14f * this->timer);
	setScale(f, f);
	setRotation(r);
}