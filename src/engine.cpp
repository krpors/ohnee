#include <cassert>
#include <iostream>
#include <sstream>

#include "engine.hpp"
#include "state.hpp"
#include "introstate.hpp"
#include "pausestate.hpp"
#include "playstate.hpp"
#include "statestack.hpp"
#include "util.hpp"

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

void Engine::run() {
	Text fpsText(this->fontSmall);
	FpsCounter fps;

	sf::ContextSettings ctx;
	ctx.antialiasingLevel = 4;

	// Initialize the possible states of the game here plx.
	this->renderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024, 768), "OHNEE v0.0.1", sf::Style::Close, ctx);
	this->renderWindow->setVerticalSyncEnabled(true);
	this->renderWindow->setKeyRepeatEnabled(false);

	GameState::Context context;
	context.engine = this;

	StateStack stack(context);
	stack.registerState<IntroState>(StateId::Intro);
	stack.registerState<PlayState>(StateId::Game);
	stack.registerState<PauseState>(StateId::Pause);

	stack.pushState(StateId::Intro);

	sf::Clock clock;
	while (this->renderWindow->isOpen() && !this->quit) {
		sf::Time elapsed = clock.restart();

		sf::Event event;
		while (this->renderWindow->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->renderWindow->close();
			}

			stack.handleEvent(event);
		}

		this->renderWindow->clear();
		fps.update(elapsed);
		stack.update(elapsed);

		this->renderWindow->draw(stack);

		std::stringstream ss;
		ss << "FPS: " << fps.getFps();
		fpsText.setText(ss.str());
		this->renderWindow->draw(fpsText);

		this->renderWindow->display();

		stack.applyChanges();
	}
}
