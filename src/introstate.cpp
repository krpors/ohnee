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
	   << "Press P to play." << std::endl
	   << "Press Q to quit.";
	text.setText(ss.str());
	text.setPosition( { 20, 100} );

	Button btn;
	btn.setPosition({100, 100});

	Button btn2;
	btn2.setPosition({ 100, 130});

	this->buttonContainer.addButton(btn);
	this->buttonContainer.addButton(btn2);
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
			stateStack->pushState(StateId::Game);
		case sf::Keyboard::Up:
			this->buttonContainer.selectPrevious();
			break;
		case sf::Keyboard::Down:
			this->buttonContainer.selectNext();
			break;
		default:
			break;
		}
	}
}

void IntroState::update(const sf::Time& dt) {
	this->timeTotal += dt;
}

void IntroState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->text);
	target.draw(this->buttonContainer);
}
