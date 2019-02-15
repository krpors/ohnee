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

	Text::setText(ss.str());
	Text::setPosition(150, 200);
	// Text::setOrigin(150, 300);
}

BobbingText::~BobbingText() {

}

void BobbingText::handleInput(const sf::Event& event) {
	sf::Vector2f orig = Text::getOrigin();
	switch (event.key.code) {
	case sf::Keyboard::W: Text::setOrigin(orig.x, orig.y -= 10); break;
	case sf::Keyboard::A: Text::setOrigin(orig.x -= 10, orig.y); break;
	case sf::Keyboard::S: Text::setOrigin(orig.x, orig.y += 10); break;
	case sf::Keyboard::D: Text::setOrigin(orig.x += 10, orig.y); break;
	default:
		break;
	}

	std::cout << Text::getOrigin().x << std::endl;
}

void BobbingText::update(const sf::Time& dt) {
	this->timer += dt.asSeconds();
	float f = 2.2f * fabs(sin(3.14f * this->timer)) + 1.0f;
	float r = 20.0f * cos(3.14f * this->timer);
	setScale(f, f);
	setRotation(r);
}