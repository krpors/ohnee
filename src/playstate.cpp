#include <cassert>
#include <cmath>
#include <iostream>

#include "playstate.hpp"
#include "pausestate.hpp"
#include "util.hpp"

PlayState::PlayState(StateStack& stack, GameState::Context context) :
GameState(stack, context),
window(context.window) {

	assert(window != NULL);

	this->p.setColor(sf::Color::Red);
	this->other.setColor(sf::Color::Green);

}

PlayState::~PlayState() {
	TRACE("");
}

void PlayState::handleInput(const sf::Event& event) {

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->p.unmove();
			this->stateStack->pushState(StateId::Pause);
			break;
		default: break;
		}
	}

	p.handleInput(event);
}

/*

bounce:

a * cos(f * x * 2 * pi) / e^(d * x)

a = amplitude
f = frequency
x = time
d = decay

*/

void PlayState::update(const sf::Time& dt) {
	this->dt += dt;

	this->p.update(dt);
	this->other.update(dt);

	// bool lol = std::addressof(this->p) == std::addressof(this->other);
	// TRACE("eq: " << std::boolalpha << lol);

	// TODO: with more than 2 players this will become sucky, so refactor the
	// isCollidingWithSelf() etc.
	if (this->p.isColliding(this->other)) {
		this->p.die();
	}

	if (this->other.isColliding(this->p)) {
		this->other.die();
	}
}

void PlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->p, states);
	target.draw(this->other, states);
}
