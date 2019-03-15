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

PlayState::PlayState(Engine* const engine) : engine(engine) {
	std::cout << "PlayState()" << std::endl;
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
		case sf::Keyboard::Escape: this->engine->setQuit(true); break;
		default: break;
		}
	}

	p.handleInput(event);
}

void PlayState::update(const sf::Time& dt) {
	p.update(dt);
}

void PlayState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(p, states);
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

void Engine::run() {
	Text fpsText(this->fontSmall);
	FpsCounter fps;

	PlayState state(this);

	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 4;

	this->renderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "OHNEE v0.0.1", sf::Style::Close, ctx);
	this->renderWindow->setVerticalSyncEnabled(true);

	sf::Clock clock;
	while (this->renderWindow->isOpen() && !this->quit) {
		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (this->renderWindow->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->renderWindow->close();
			}

			state.handleInput(event);
		}

		this->renderWindow->clear();
		fps.update(elapsed);
		state.update(elapsed);
		this->renderWindow->draw(state);

		std::stringstream ss;
		ss << "FPS: " << fps.getFps();
		fpsText.setText(ss.str());
		this->renderWindow->draw(fpsText);

		this->renderWindow->display();
	}
}