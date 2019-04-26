#include <sstream>

#include "introstate.hpp"

// Example of a nice clean menu:
// https://media.indiedb.com/images/articles/1/81/80146/auto/title-menu.png

// =============================================================================

/**
 * Select an element randomly given a start and end iterator.
 * Returns the Iter.begin().
 */
template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(gen));

	return start;
}

// =============================================================================

IntroState::IntroState(StateStack& stack, GameState::Context context) :
GameState(stack, context) {

	this->introtexts = {
		"The newt bites you. You die...",
		"Segmentation fault",
		"Core dumped.",
		"You've got mail. Check your inbox!",
		"Blue screen of death imminent... Or Linux?",
		"Grab them by the pony.",
		"e = 2.7182818284590452353602874...",
		"pi = 3.1415926535897932384626433...",
		"This game is bugfree!",
		"Use the source, Luke.",
		"i = 0x5f3759df - (i >> 1); // what the f?",
		"We have broken SHA-1 in practice.",
		"When in doubt, print it out!",
		"Bogosort is the best sort!",
		"Fatal Error: NO_ERROR",
		"It's true! It's fantastic. It's great!",
		"Random number selected: 42",
		"Tabs for indentation, spaces for alignment!",
		"Kernel panic!",
		"Great balls of fire!",
		"Derp de derp de tiddely derpy derpy dum.\nRated PG-13.",
		"Click HERE to download more RAM!",
		"HOW MUCH DEDODATED WAM\nDO I NEED TO RUN SERVER\?\?\?!",
		"Practice & planning\nprevent piss poor performance.",
		"Oh sweet summer child...",
		"STABLE GENIUS!",
		"const sf::Time& dt;"
	};

	this->text.setFont(this->context.engine->getFontLarge());
	this->text.setKerning(2);
	this->text.setText(*select_randomly(this->introtexts.begin(), this->introtexts.end()));
	this->text.setPosition( { 150, 200} );
	this->text.setRotation(-5.0f);
	this->text.setFunction([] (const sf::Time& dt) {
		return 0.5f * fabs(sin(M_PI * dt.asSeconds())) + 1.0f;
	});

	const sf::RenderWindow& win = this->context.engine->getRenderWindow();
	int buttonWidth = 200;
	float centerx = (win.getSize().x / 2) - (buttonWidth / 2);

	auto btnPlay = std::make_shared<Button>(this->context.engine->getFontLarge());
	btnPlay->setText("Play da game!");
	btnPlay->setPosition({centerx, 300});
	btnPlay->setCallback([this] {
		TRACE("Play");
		this->stateStack->pushState(StateId::Game);
	});

	auto btnOptions = std::make_shared<Button>(this->context.engine->getFontLarge());
	btnOptions->setText("Options");
	btnOptions->setPosition({ centerx + 10, 330 });
	btnOptions->setCallback([this] {
		TRACE("Options!");
	});

	auto btnQuit = std::make_shared<Button>(this->context.engine->getFontLarge());
	btnQuit->setText("Quit");
	btnQuit->setPosition({ centerx + 20, 360 });
	btnQuit->setCallback([this] {
		this->context.engine->setQuit(true);
		TRACE("Quit");
	});

	this->buttonContainer.addButton(btnPlay);
	this->buttonContainer.addButton(btnOptions);
	this->buttonContainer.addButton(btnQuit);

	if (!this->music.openFromFile("./media/distressed.ogg")) {
		TRACE("Music could not be loaded.");
	}

	if (!this->bufSoundSelect.loadFromFile("./media/select.wav")) {
		TRACE("Sound could not be loaded");
	}

	if (!this->bufSoundOk.loadFromFile("./media/validate.wav")) {
		TRACE("Could not load ok sound");
	}

	for (int i = 0; i < 10; i++) {
		sf::Sound snd;
		snd.setBuffer(this->bufSoundSelect);
		this->soundList.push_back(snd);
		TRACE("Added sound " << i);
	}

	this->soundSelect.setBuffer(this->bufSoundSelect);
	this->soundOk.setBuffer(this->bufSoundOk);
}

IntroState::~IntroState() {
}

void IntroState::activate() {
	this->music.play();
}

void IntroState::deactivate() {
	this->music.stop();
}

void IntroState::handleInput(const sf::Event& event) {
	if (event.type != sf::Event::KeyPressed) {
		return;
	}

	TRACE("Handling input (key " << event.key.code << ")");

	switch (event.key.code) {
	case sf::Keyboard::Up:
		this->buttonContainer.selectPrevious();
		this->soundList.next().play();
		break;
	case sf::Keyboard::Down:
		this->buttonContainer.selectNext();
		this->soundList.next().play();
		break;
	case sf::Keyboard::Return:
		this->buttonContainer.activate();
		this->soundOk.play();
		break;
	case sf::Keyboard::Escape:
		this->text.setText(*select_randomly(this->introtexts.begin(), this->introtexts.end()));
		break;
	case sf::Keyboard::A:
		this->text.setFunction([] (const sf::Time& dt) {
			return 0.5f * fabs(sin(M_PI * dt.asSeconds())) + 1.0f;
		});
		break;
	case sf::Keyboard::S:
		this->text.setFunction([] (const sf::Time& dt) {
			return 1.5f * fabs(sin(0.1 * M_PI * dt.asSeconds())) + 1.0f;
		});
		break;
	default:
		break;
	}
}

void IntroState::update(const sf::Time& dt) {
	this->timeTotal += dt;

	this->text.update(dt);
	this->buttonContainer.update(dt);
}

void IntroState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->text);
	target.draw(this->buttonContainer);
}
