#include <cassert>
#include <iostream>
#include <sstream>

#include "engine.hpp"
#include "util.hpp"

GameState::GameState() {
	debug_print("%s", "GameState()");
}

GameState::~GameState() {
	debug_print("%s", "~GameState()");
}

// =============================================================================

PauseState PauseState::instance;

PauseState::PauseState() {
}

PauseState::~PauseState() {
	debug_print("%s", "~PauseState()");
}

void PauseState::setEngine(Engine* const engine) {
	this->engine = engine;
	this->pauseText.setFont(this->engine->getFontLarge());
	this->pauseText.setPosition(80, 80);
	this->pauseText.setText("PAUSED!!! Press Q now to quit.");
}

void PauseState::init() {
	sf::Texture texture;
	texture.create(800, 600);
	texture.update(this->engine->getRenderWindow());
	sf::Image source = texture.copyToImage();
	sf::Image target;
	target.create(800, 600);

	// TODO: This takes a shitload of time on my vbox?
	blurImage(source, target);

	this->screencapture.create(800, 600);
	this->screencapture.loadFromImage(target);
}

void PauseState::cleanup() {
}

void PauseState::handleInput(const sf::Event& event) {
	assert(this->engine != nullptr);

	// FIXME: when the init() state takes too much time, and the ESC key is pressed
	// rapidly, there are more pause states pushed, resulting in a large stack
	// of pause states! This is mitigated by setting the key repeat to false,
	// but maybe this requires a better solution.

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->engine->popState();
			break;
		case sf::Keyboard::Q:
			this->engine->setQuit(true);
			break;
		default: break;
		}
	}
}

void PauseState::update(const sf::Time& dt) {
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprite;
	// sf::Color color(255, 255, 255, 100);
	// sprite.setColor(color);
	sprite.setTexture(this->screencapture);

	target.draw(sprite, states);
	target.draw(this->pauseText, states);
}

PauseState* PauseState::getInstance() {
	return &PauseState::instance;
}

// =============================================================================

// Note: Required or else "undefined reference to `PlayState::instance'"
PlayState PlayState::instance;

PlayState::PlayState() {
}

PlayState::~PlayState() {
	debug_print("%s", "~PlayState()");
}

void PlayState::setEngine(Engine* const engine) {
	this->engine = engine;
	this->text.setFont(engine->getFontSmall());
	this->text.setText("YO MOMMA!");
}

void PlayState::init() {
}

void PlayState::cleanup() {
}

void PlayState::handleInput(const sf::Event& event) {
	assert(this->engine != nullptr);

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			this->engine->pushState(PauseState::getInstance());
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

PlayState* PlayState::getInstance() {
	return &PlayState::instance;
}

// =============================================================================

Engine::Engine() {
	std::string chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-=_+|/\\:;'\"<>,.?";

	this->fontSmall = std::make_shared<ImageFont>("font.png", chars);
	this->fontSmall->setKerning(2);

	this->fontLarge = std::make_shared<ImageFont>("font-large.png", chars);
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
	PlayState* statePlay = PlayState::getInstance();
	statePlay->setEngine(this);

	PauseState* statePause = PauseState::getInstance();
	statePause->setEngine(this);

	this->stateStack.push(statePlay);

	this->renderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "OHNEE v0.0.1", sf::Style::Close, ctx);
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