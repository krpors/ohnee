#include <cmath>
#include <memory>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "bob.hpp"
#include "text.hpp"

BobbingText::BobbingText() {

}

BobbingText::BobbingText(const std::shared_ptr<ImageFont>& font) :
		Text(font) {

	this->timer = 0.0f;
	std::stringstream ss;
	ss << "I am currently watching" << std::endl
	   << "the Tragedy of systemd." << std::endl
	   << "!@#$%^&*()-=_+|/\\:;'\"<>,.?" << std::endl;

	Text::setPosition(150, 200);
	Text::setText(ss.str());
}

BobbingText::~BobbingText() {

}

void BobbingText::handleInput(const sf::Event& event) {
	sf::Vector2f orig = Text::getOrigin();
	sf::Vector2f pos = Text::getPosition();

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::W: Text::setOrigin(orig.x, orig.y -= 10); break;
		case sf::Keyboard::A: Text::setOrigin(orig.x -= 10, orig.y); break;
		case sf::Keyboard::S: Text::setOrigin(orig.x, orig.y += 10); break;
		case sf::Keyboard::D: Text::setOrigin(orig.x += 10, orig.y); break;

		case sf::Keyboard::K: Text::setPosition(pos.x, pos.y -= 10); break;
		case sf::Keyboard::H: Text::setPosition(pos.x -= 10, pos.y); break;
		case sf::Keyboard::J: Text::setPosition(pos.x, pos.y += 10); break;
		case sf::Keyboard::L: Text::setPosition(pos.x += 10, pos.y); break;
		default:
			break;
		}
	}
}

void BobbingText::update(const sf::Time& dt) {
	this->timer += dt.asSeconds();
	float f = 1.2f * fabs(sin(3.14f * this->timer)) + 1.0f;
	setScale(f, f);
}