#include <cassert>

#include "pausestate.hpp"
#include "statestack.hpp"
#include "util.hpp"

PauseState::PauseState(StateStack& stack, GameState::Context context) :
	GameState(stack, context) {

	this->init();
}

void PauseState::init() {
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
}

void PauseState::cleanup() {

}

void PauseState::handleInput(const sf::Event& event) {
	// FIXME: when the init() state takes too much time, and the ESC key is pressed
	// rapidly, there are more pause states pushed, resulting in a large stack
	// of pause states! This is mitigated by setting the key repeat to false,
	// but maybe this requires a better solution.

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->stateStack->popState();
			// this->engine->popState();
			break;
		case sf::Keyboard::R:
			// PlayState::getInstance()->init();
			break;
		case sf::Keyboard::F:
			break;
		case sf::Keyboard::Q:
			GameState::context.engine->setQuit(true);
			// this->engine->setQuit(true);
			break;
		default: break;
		}
	}
}

void PauseState::update(const sf::Time& dt) {
	// this->pauseText.update(dt);
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	sf::Color color(255, 255, 255, 100);
	sprite.setColor(color);
	sprite.setTexture(this->screencapture);

	target.draw(sprite, states);
	// target.draw(this->pauseText, states);
}