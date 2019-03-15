#include <cassert>
#include <iostream>
#include <sstream>

#include "engine.hpp"
#include "util.hpp"

GameState::GameState() {
	std::cout << "GameState()" << std::endl;
}

GameState::~GameState() {
	std::cout << "~GameState()" << std::endl;
}

// =============================================================================

PauseState PauseState::instance;

PauseState::PauseState() {
}

PauseState::~PauseState() {
}

void PauseState::init(Engine* const engine) {
	this->engine = engine;
	this->pauseText.setFont(this->engine->getFontSmall());
	this->pauseText.setPosition(80, 80);
	this->pauseText.setText("PAUSED!!!");
}

void PauseState::handleInput(const sf::Event& event) {
	assert(this->engine != nullptr);

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->engine->popState();
			break;
		default: break;
		}
	}
}

void PauseState::update(const sf::Time& dt) {
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->pauseText, states);
}

PauseState* PauseState::getInstance() {
	return &PauseState::instance;
}

// =============================================================================

// Note: Required or else "undefined reference to `PlayState::instance'"
PlayState PlayState::instance;

PlayState::PlayState() {
	std::cout << "PlayState()" << std::endl;
}

void PlayState::init(Engine* const engine) {
	this->engine = engine;
	this->text.setFont(engine->getFontSmall());
	this->text.setText("YO MOMMA!");
}

PlayState::~PlayState() {
	std::cout << "~PlayState()" << std::endl;
}

void PlayState::handleInput(const sf::Event& event) {
	assert(this->engine != nullptr);

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->engine->pushState(PauseState::getInstance());
			break;
		// 1. push pause/menu state on the engine stack
		// 2. draw the top of the stack
		// 3. see PauseState handleInput
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

PlayState* PlayState::getInstance() {
	return &PlayState::instance;
}

// =============================================================================

Engine::Engine() {
	this->fontSmall = std::make_shared<ImageFont>(
		"font.png",
		" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?"
	);
	this->fontSmall->setKerning(2);
}

Engine::~Engine() {
}

const std::shared_ptr<ImageFont>& Engine::getFontSmall() const {
	return this->fontSmall;
}

void Engine::setQuit(bool quit) {
	this->quit = quit;
}

void Engine::pushState(GameState* const state) {
	this->stateStack.push(state);
}

void Engine::popState() {
	this->stateStack.pop();
}

void Engine::run() {
	Text fpsText(this->fontSmall);
	FpsCounter fps;

	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 4;

	// Initialize the possible states of the game here plx.
	PlayState* statePlay = PlayState::getInstance();
	statePlay->init(this);

	PauseState* statePause = PauseState::getInstance();
	statePause->init(this);

	this->stateStack.push(statePlay);

	this->renderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "OHNEE v0.0.1", sf::Style::Close, ctx);
	this->renderWindow->setVerticalSyncEnabled(true);

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