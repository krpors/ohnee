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

	auto btnPlay = std::make_shared<Button>(this->context.engine->getFontSmall());
	btnPlay->setText("Play da game!");
	btnPlay->setPosition({100, 100});
	btnPlay->setCallback([this] {
		TRACE("Play");
		this->stateStack->pushState(StateId::Game);
	});

	auto btnOptions = std::make_shared<Button>(this->context.engine->getFontSmall());
	btnOptions->setText("Options");
	btnOptions->setPosition({ 100, 130 });
	btnOptions->setCallback([this] {
		TRACE("Options!");
	});

	auto btnQuit = std::make_shared<Button>(this->context.engine->getFontSmall());
	btnQuit->setText("Quit");
	btnQuit->setPosition({ 100, 160 });
	btnQuit->setCallback([this] {
		this->context.engine->setQuit(true);
		TRACE("Quit");
	});

	this->buttonContainer.addButton(btnPlay);
	this->buttonContainer.addButton(btnOptions);
	this->buttonContainer.addButton(btnQuit);
}

void IntroState::cleanup() {
}

void IntroState::handleInput(const sf::Event& event) {
	if (event.type != sf::Event::KeyPressed) {
		return;
	}

	TRACE("Handling input (key " << event.key.code << ")");

	switch (event.key.code) {
	case sf::Keyboard::Up:
		this->buttonContainer.selectPrevious();
		break;
	case sf::Keyboard::Down:
		this->buttonContainer.selectNext();
		break;
	case sf::Keyboard::Return:
		this->buttonContainer.activate();
		break;
	default:
		break;
	}
}

void IntroState::update(const sf::Time& dt) {
	this->timeTotal += dt;

	this->buttonContainer.update(dt);
}

void IntroState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->text);
	target.draw(this->buttonContainer);
}
