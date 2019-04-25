#include <cassert>
#include <iostream>

#include "pausestate.hpp"
#include "statestack.hpp"
#include "util.hpp"

PauseState::PauseState(StateStack& stack, GameState::Context context) :
	GameState(stack, context) {

	this->pauseText.setFont(this->context.engine->getFontLarge());
	this->pauseText.setKerning(8);

	std::string crap = "The game is paused\n";
	crap += "Press Q now to quit to main menu!";
	this->pauseText.setText(crap);

	this->pauseText.setPosition(200, 150);
	this->pauseText.setRotation(-15);

	const sf::RenderWindow& window = this->context.engine->getRenderWindow();
	sf::Vector2u winSize = window.getSize();

	sf::Texture texture;
	texture.create(winSize.x, winSize.y);
	texture.update(window);
	sf::Image source = texture.copyToImage();
	sf::Image target;
	target.create(winSize.x, winSize.y);

	// TODO: This takes a shitload of time on my vbox?
	blurImage(source, target);

	this->screencapture.create(winSize.x, winSize.y);
	this->screencapture.loadFromImage(target);

	this->context.window->setView(this->context.window->getDefaultView());
}

PauseState::~PauseState() {
	TRACE("Destroying pause state");
}

void PauseState::handleInput(const sf::Event& event) {
	// Only respond to keypresses for now.
	if (event.type != sf::Event::KeyPressed) {
		return;
	}

	switch (event.key.code) {
	case sf::Keyboard::Escape:
		this->stateStack->popState();
		break;
	case sf::Keyboard::R:
		break;
	case sf::Keyboard::F:
		break;
	case sf::Keyboard::Q:
		this->stateStack->popState(); // pop the pause state
		this->stateStack->popState(); // pop the play state
		break;
	default: break;
	}
}

void PauseState::update(const sf::Time& dt) {
	this->pauseText.update(dt);
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sf::Color color(255, 255, 255, 100);
	sprite.setColor(color);
	sprite.setTexture(this->screencapture);

	target.draw(sprite, states);
	target.draw(this->pauseText, states);
}
