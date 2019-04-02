#include <sstream>

#include "introstate.hpp"

// Example of a nice clean menu:
// https://media.indiedb.com/images/articles/1/81/80146/auto/title-menu.png

IntroState::IntroState(StateStack& stack, GameState::Context context) :
GameState(stack, context) {

	this->init();
}

IntroState::~IntroState() {
}

void IntroState::init() {
	text.setFont(this->context.engine->getFontLarge());
	std::stringstream ss;
	ss << "Welcome to OHNEE!" << std::endl
	   << "Press P to play.";
	text.setText(ss.str());
	text.setPosition( { 20, 100} );
}

void IntroState::cleanup() {
}

void IntroState::handleInput(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Q:
			this->context.engine->setQuit(true);
			break;
		case sf::Keyboard::P:
			stateStack->pushState<PlayState>();
		default:
			break;
		}
	}
}

void IntroState::update(const sf::Time& dt) {
	this->timeTotal += dt;
}

void IntroState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(text);
}
