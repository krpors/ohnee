#include <cassert>
#include <cmath>
#include <iostream>

#include "playstate.hpp"
#include "pausestate.hpp"
#include "util.hpp"

ShakeView::ShakeView() :
shakeTimer(sf::Time::Zero),
shaking(false),
shakeFactor(0.0f) {
}

ShakeView::ShakeView(const sf::View& other) :
shakeTimer(sf::Time::Zero),
shaking(false),
shakeFactor(0.0f) {

	setCenter(other.getCenter());
	setRotation(other.getRotation());
	setSize(other.getSize());
	setViewport(other.getViewport());

	this->original = other;
}

void ShakeView::setShaking(bool shaking) {
	this->shaking = shaking;
	this->shakeTimer = sf::Time::Zero;
	this->shakeFactor = 0.0f;

	if (!this->shaking) {
		// When shaking should stop, reset the center to the original values.
		this->setCenter(this->original.getCenter());
	}
}

void ShakeView::update(const sf::Time& dt) {
	// If we're shaking, and the timer is still under 3 seconds, do some shakin'
	if (this->shaking && this->shakeTimer.asSeconds() < 3.0f) {
		this->shakeTimer += dt;
		float x           = this->shakeTimer.asSeconds();
		float amplitude   = 30.0f;
		float frequency   = 8.0f;
		float decay       = 2.0f;
		this->shakeFactor = amplitude * (std::cos(2 * M_PI * frequency * x) / std::exp(decay * x));
		this->shakeFactor = std::fabs(this->shakeFactor);

		this->setCenter(this->original.getCenter().x + shakeFactor, this->original.getCenter().y + shakeFactor);
	}
}

// =============================================================================

PlayState::PlayState(StateStack& stack, GameState::Context context) :
GameState(stack, context),
window(context.window) {

	assert(window != NULL);

	this->p.setColor(sf::Color::Red);
	this->other.setColor(sf::Color::Green);
	this->shakeView = this->window->getDefaultView();

	TRACE(this->shakeView.getCenter().x << ", " << this->shakeView.getCenter().y);
	TRACE(this->window->getDefaultView().getCenter().x << ", " << this->window->getDefaultView().getCenter().y);
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

void PlayState::update(const sf::Time& dt) {
	this->p.update(dt);
	this->other.update(dt);
	this->shakeView.update(dt);
	this->window->setView(this->shakeView);

	// TODO: with more than 2 players this will become sucky, so refactor the
	// isCollidingWithSelf() etc.
	// TODO: shake the screen when colliding with self. Also: callbacks?

	if (this->p.isColliding(this->other) && !this->p.isDead()) {
		this->p.die();
		this->shakeView.setShaking(true);
		TRACE("P1 is dead!");
	}

	if (this->other.isColliding(this->p) && !this->other.isDead()) {
		this->other.die();
		this->shakeView.setShaking(true);
		TRACE("P2 is dead!");
	}
}

void PlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->p, states);
	target.draw(this->other, states);
}
