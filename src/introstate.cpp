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

	const sf::RenderWindow& win = this->context.engine->getRenderWindow();
	int buttonWidth = 200;
	float centerx = (win.getSize().x / 2) - (buttonWidth / 2);

	auto btnPlay = std::make_shared<Button>(this->context.engine->getFontLarge());
	btnPlay->setText("Play da game!");
	btnPlay->setPosition({centerx, 100});
	btnPlay->setCallback([this] {
		TRACE("Play");
		this->stateStack->pushState(StateId::Game);
	});

	auto btnOptions = std::make_shared<Button>(this->context.engine->getFontLarge());
	btnOptions->setText("Options");
	btnOptions->setPosition({ centerx + 10, 130 });
	btnOptions->setCallback([this] {
		TRACE("Options!");
	});

	auto btnQuit = std::make_shared<Button>(this->context.engine->getFontLarge());
	btnQuit->setText("Quit");
	btnQuit->setPosition({ centerx + 20, 160 });
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
