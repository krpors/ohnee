#include <cassert>

#include "playstate.hpp"
#include "pausestate.hpp"
#include "util.hpp"

PlayState::PlayState(StateStack& stack, GameState::Context context) :
	GameState(stack, context) {
}

PlayState::~PlayState() {
}

void PlayState::init() {
	this->p.reset();
}

void PlayState::cleanup() {
}

void PlayState::handleInput(const sf::Event& event) {

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->p.unmove();
			this->stateStack->pushState<PauseState>();
			break;
		default: break;
		}
	}

	p.handleInput(event);
}

void PlayState::update(const sf::Time& dt) {
	this->p.update(dt);
}

void PlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->p, states);
}
