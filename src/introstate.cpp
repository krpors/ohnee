#include "introstate.hpp"

// Example of a nice clean menu:
// https://media.indiedb.com/images/articles/1/81/80146/auto/title-menu.png

IntroState::IntroState(StateStack& stack, GameState::Context context) :
	GameState(stack, context) {
}

void IntroState::init() {
}

void IntroState::cleanup() {
}

void IntroState::handleInput(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			// FIXME: when popping the state, the update call in will fail
			// because there are no states anymore. So "schedule" them instead.
			stateStack->popState();
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
