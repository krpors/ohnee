#include <sstream>
#include <fstream>

#include "introstate.hpp"

// Example of a nice clean menu:
// https://media.indiedb.com/images/articles/1/81/80146/auto/title-menu.png

// =============================================================================

/**
 * Select an element randomly given a start and end iterator.
 * Returns the Iter.begin().
 *
 * FIXME: currently, for this to work, a collection must not be empty.
 * Dereferencing the iterator obviously will segfault.
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

// The filename which is used to read the introductory bobbing texts.
const std::string IntroState::INTROTEXTS_FILENAME = "./media/introtexts.txt";

IntroState::IntroState(StateStack& stack, GameState::Context context) :
GameState(stack, context) {

	this->readIntroTexts();

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

void IntroState::readIntroTexts() {
	std::ifstream fs;
	fs.open(INTROTEXTS_FILENAME);
	if (!fs) {
		// To prevent major malfunctions with select_randomly, we insert
		// at least one text.
		TRACE("Could not read intro texts from file " << INTROTEXTS_FILENAME);
		this->introtexts.push_back("!COULD NOT READ INTROTEXTS.TXT!");
		return;
	}

	this->introtexts.clear();

	// Rather naive implementation I reckon. If a slash is supplied
	// at the end of the line in the file, bad things will probably happen
	// due to the indexing of s[i + 1].
	std::string s;
	while (std::getline(fs, s)) {
		std::string buf;
		for (std::string::size_type i = 0; i < s.length(); i++) {
			// Is the current character a backslash, followd by n? Then
			// append an actual newline to the string buf.
			if (s[i] == '\\' && s[i + 1] == 'n') {
				buf += '\n';
				// skip one iteration.
				i++;
			} else {
				buf += s[i];
			}
		}

		this->introtexts.push_back(buf);
	}

}
