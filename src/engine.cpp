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

// Example of a nice clean menu:
// https://media.indiedb.com/images/articles/1/81/80146/auto/title-menu.png

IntroState IntroState::instance;

IntroState::IntroState() {
}

IntroState::~IntroState() {
}

void IntroState::init() {
}

void IntroState::cleanup() {
}

void IntroState::handleInput(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
		case sf::Keyboard::Escape:
			break;
		default:
			break;
		}
	}
}

void IntroState::update(const sf::Time& dt) {
	this->timeTotal += dt;

	if (this->timeTotal >= sf::milliseconds(50)) {
		pos.x = Rng::distPos() * 1024;
		pos.y = Rng::distPos() * 768;
		this->timeTotal = sf::Time::Zero;
	}
}

void IntroState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape shape;
	shape.setPosition(this->pos);
	shape.setFillColor(sf::Color::Blue);
	shape.setSize({ 20, 20 });

	target.draw(shape);
}

IntroState* IntroState::getInstance() {
	return &IntroState::instance;
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
	sf::Vector2u winSize = this->engine->getRenderWindow().getSize();

	sf::Texture texture;
	texture.create(winSize.x, winSize.y);
	texture.update(this->engine->getRenderWindow());
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
		case sf::Keyboard::R:
			PlayState::getInstance()->init();
			break;
		case sf::Keyboard::F:
			break;
		case sf::Keyboard::Q:
			this->engine->setQuit(true);
			break;
		default: break;
		}
	}
}

void PauseState::update(const sf::Time& dt) {
	this->pauseText.update(dt);
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
	this->p.reset();
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