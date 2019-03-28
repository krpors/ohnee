#include <cassert>

#include "playstate.hpp"
#include "pausestate.hpp"
#include "util.hpp"

PlayState::PlayState(StateStack& stack) :
	GameState(stack) {
}

void PlayState::setEngine(Engine* const engine) {
	this->engine = engine;
	this->text.setFont(engine->getFontSmall());
	this->text.setText("YO MOMMA!");
}

void PlayState::init() {
	this->p.reset();
}

void PlayState::cleanup() {
}

void PlayState::handleInput(const sf::Event& event) {
	assert(this->engine != nullptr);

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			// this->engine->pushState(PauseState::getInstance());
			break;
		default: break;
		}
	}

	p.handleInput(event);
}

void PlayState::update(const sf::Time& dt) {
	this->p.update(dt);
	this->text.setPosition({ p.getPosition().x + 10.0f, p.getPosition().y - 10.0f});
}

void PlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->p, states);
	target.draw(this->text, states);
}