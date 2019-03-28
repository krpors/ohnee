#include "introstate.hpp"

// Example of a nice clean menu:
// https://media.indiedb.com/images/articles/1/81/80146/auto/title-menu.png

IntroState IntroState::instance;

IntroState::IntroState() {
}

IntroState::~IntroState() {
}

void IntroState::init() {
}

void IntroState::cleanup() {
}

void IntroState::handleInput(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			break;
		default:
			break;
		}
	}
}

void IntroState::update(const sf::Time& dt) {
	this->timeTotal += dt;

	if (this->timeTotal >= sf::milliseconds(50)) {
		pos.x = Rng::distPos() * 1024;
		pos.y = Rng::distPos() * 768;
		this->timeTotal = sf::Time::Zero;
	}
}

void IntroState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape shape;
	shape.setPosition(this->pos);
	shape.setFillColor(sf::Color::Blue);
	shape.setSize({ 20, 20 });

	target.draw(shape);
}

IntroState* IntroState::getInstance() {
	return &IntroState::instance;
}
