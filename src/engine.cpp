#include <cassert>
#include <iostream>
#include <sstream>

#include "engine.hpp"
#include "state.hpp"
#include "introstate.hpp"
#include "pausestate.hpp"
#include "playstate.hpp"
#include "util.hpp"

// =============================================================================


// =============================================================================

// =============================================================================

Engine::Engine() {
	std::string chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?";

	this->fontSmall = std::make_shared<ImageFont>("./media/font.png", chars);
	this->fontSmall->setKerning(2);

	this->fontLarge = std::make_shared<ImageFont>("./media/font-large.png", chars);
	this->fontLarge->setKerning(2);
}

Engine::~Engine() {
}

const std::shared_ptr<ImageFont> Engine::getFontSmall() const {
	return this->fontSmall;
}

const std::shared_ptr<ImageFont> Engine::getFontLarge() const {
	return this->fontLarge;
}

const sf::RenderWindow& Engine::getRenderWindow() const {
	return *this->renderWindow;
}

void Engine::setQuit(bool quit) {
	this->quit = quit;
}

void Engine::pushState(GameState* const state) {
	state->init();
	this->stateStack.push(state);
	debug_print("GameState stack size is now at size %li", this->stateStack.size());
}

void Engine::initState() {
	this->stateStack.top()->init();
}

void Engine::popState() {
	this->stateStack.pop();
	debug_print("GameState stack size is now at size %li", this->stateStack.size());
}

void Engine::run() {
	Text fpsText(this->fontSmall);
	FpsCounter fps;

	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 4;

	// Initialize the possible states of the game here plx.
	IntroState* introState = IntroState::getInstance();

	PlayState* statePlay = PlayState::getInstance();
	statePlay->setEngine(this);

	PauseState* statePause = PauseState::getInstance();
	statePause->setEngine(this);

	this->stateStack.push(introState);

	this->renderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024, 768), "OHNEE v0.0.1", sf::Style::Close, ctx);
	this->renderWindow->setVerticalSyncEnabled(true);
	this->renderWindow->setKeyRepeatEnabled(false);

	sf::Clock clock;
	while (this->renderWindow->isOpen() && !this->quit) {
		GameState* currentState = this->stateStack.top();

		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (this->renderWindow->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->renderWindow->close();
			}

			currentState->handleInput(event);
		}

		this->renderWindow->clear();
		fps.update(elapsed);
		currentState->update(elapsed);
		this->renderWindow->draw(*currentState);

		std::stringstream ss;
		ss << "FPS: " << fps.getFps();
		fpsText.setText(ss.str());
		this->renderWindow->draw(fpsText);

		this->renderWindow->display();
	}
}